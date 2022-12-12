#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

class PID{
  public:
	double error;
	double sample;
	double lastSample;
	double kP, kI, kD;      
	double P, I, D;
	double pid;
	
	double setPoint;
	long lastProcess;
	
	PID(double _kP, double _kI, double _kD){
		kP = _kP;
		kI = _kI;
		kD = _kD;
    pid=50;
	}
	
	void addNewSample(double _sample){
		sample = _sample;
	}
	
	void setSetPoint(double _setPoint){
		setPoint = _setPoint;
	}
	
	double process(){
		// Implementação P ID
		error = setPoint - sample;
		float deltaTime = (millis() - lastProcess) / 1000.0;
		lastProcess = millis();
		
		//P
		P = error * kP;
		
		//I
		I = I + (error * kI) * deltaTime;
		
		//D
		D = (lastSample - sample) * kD / deltaTime;
		lastSample = sample;
		
		// Soma tudo
		pid = P + I + D;
		
		return pid;
	}
};
// Definição de componentes
  int botao1=6,
      botao2=7,
      sensor=A0,
      pot=A1;
  Servo s;
  LiquidCrystal_I2C lcd(0x27,16,2);

// Variaveis de manipulações
  int c = -1,
      col = 0,
      abertura,
      minimo=75,
      maximo=165,
      controle=0;
  float dT,
        adminTTL;
  String login = "",
         user = "0",
         admin = "10",
         potVazao="";
  long tBotao,
       tAdmin;
  bool authV=false,
      adminMode=false;



// Outros
  PID meuPid(0.3765, 0, 0);


// Vazão
  volatile unsigned long int n_cicles = 0;
  volatile double frequency = 0;
  volatile boolean init_flag = false;

  double ajustSensorYF21(double x){
    // Daria pra substituir esses valores por 7.488
    // O valor maximo de vazão encontrado anteriormente foi 262,08 L/h que dá x=35
    //double res = x*(2.08/1000)*60*60;//2400 
    double res = x*6.82;
    return res;
  }
  void treatPulse(){
    if(init_flag){
      n_cicles++;
    }
  }

void setup() {
  Serial.begin(9600);
  // Inicializando o display
    lcd.init();
    lcd.setBacklight(HIGH);
    lcd.print("Digite o usuario");
    lcd.setCursor(0, 1);

  // Inicializando o servo
    s.attach(13);
    s.write(minimo);
  
  // Definição da pinagem
    pinMode(botao1,INPUT_PULLUP);
    pinMode(botao2,INPUT_PULLUP);
    pinMode(pot, INPUT);
    attachInterrupt(1, treatPulse, RISING);/*porta 3*/
  
}

void loop() {
  dT=(millis()-tBotao)/1000;
  adminTTL=(millis()-tAdmin)/1000;     

  if((adminMode)&&(adminTTL>=2*60)){
    adminMode=false;
    lcd.clear();
    lcd.print("Digite o usuario");
  }
                           
  if(!digitalRead(botao1)){
    if((dT>=1)&&(c!=-1)){
      login+=c;
      col+=1;  
      c=-1;  
    }
    if(!authV){  
      lcd.setCursor(col, 1);	
      tBotao=millis();  
      c++;
      if(c==10){
       c=0;
      }
      lcd.print(c);  
    }
    delay(400);
  }

  if((!digitalRead(botao2))||((!authV)&&(adminMode))){
    login+=c;
    if(!authV){
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("ACESSO"); 
      if((login==user)||(login==admin)||(adminMode)){
        if(login==admin){
          adminMode=true;
          tAdmin=millis();
        }
        lcd.setCursor(3, 1);
        lcd.print("AUTORIZADO"); 
        delay(1000);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("SETPOINT:");
        login = "";
        c=0;
        while(digitalRead(botao2)){
          potVazao = map(analogRead(pot),0,1023,239,-1);
          lcd.setCursor(11, 0);
          lcd.print(potVazao+"    ");
          Serial.print("pot: ");
          Serial.print(pot);
          Serial.print(" ");
          Serial.print("potVazao: ");
          Serial.print(potVazao);
          Serial.print(" ");
          Serial.print("adminTTL:");
          Serial.println((millis()-tAdmin)/1000);
          delay(10);
        }
        // Lendo a vazão
           n_cicles = 0;
           frequency = 0;         
           init_flag = true;         
           delay(1000);
           init_flag = false;
           frequency = n_cicles * 1.0;  
           meuPid.addNewSample((int)ajustSensorYF21(frequency)); 

        meuPid.setSetPoint(potVazao.toDouble());
        meuPid.pid =50;
        delay(500);
        while(meuPid.pid != 0)
        {
          lcd.setCursor(4, 1);
          lcd.print("AGUARDE");
          meuPid.process(); 
          delay(200);
          int controle = round(meuPid.pid)*-1; // + (int)s.read()); 
          controle=controle + s.read();
          if(controle >= maximo){
            controle = maximo;          
          }    
          else if (controle <= minimo){
            controle = minimo;
          }
          s.write(controle);

          delay(500);
          //Lendo a vazão novamente 
           n_cicles = 0;
           frequency = 0;         
           init_flag = true;         
           delay(1000);
           init_flag = false;
           frequency = n_cicles * 1.0;  
           meuPid.addNewSample((int)ajustSensorYF21(frequency)); 

          meuPid.process(); 
          if((meuPid.pid>-5)&&(meuPid.pid<5)){
            meuPid.pid=0;
          }           
          Serial.print("servo=");
          Serial.print(s.read());
          Serial.print(" ");
          Serial.print(" Setpoint: ");
          Serial.print(meuPid.setPoint);
          Serial.print(" PID: ");
          Serial.print(meuPid.pid);
          Serial.print(" Controle: ");
          Serial.print(controle);
          Serial.print(" Sample: ");
          Serial.print(meuPid.sample);
          Serial.println();
          delay(500);
        }
        lcd.setCursor(1, 1);
        lcd.print("VAZAO AJUSTADA");
        delay(2000);
        lcd.clear();
        c=-1;
        col=0;
        login="";
        if(!adminMode){
          lcd.print("Digite o usuario");   
          lcd.setCursor(0, 1);
        }
      }else{
        lcd.setCursor(5, 1);
        lcd.print("NEGADO");
        delay(1000);
        lcd.clear();
        lcd.print("Digite o usuario");
        c=-1;
        col=0;
        login="";  
      } 
      delay(500);
    }
  }

  // Isso daqui é só pra teste depois ranca fora
  Serial.print("c=");
  Serial.print(c);
  Serial.print(" ");
  Serial.print("dT=");
  Serial.print(dT);
  Serial.print(" ");
  Serial.print("login=");
  Serial.print(login);
  Serial.print(" ");
  Serial.print("col=");
  Serial.print(col);
  Serial.print(" ");
  Serial.print("servo=");
  Serial.print(s.read());
  Serial.print(" ");
  Serial.print(" Setpoint: ");
  Serial.print(meuPid.setPoint);
  Serial.print(" PID: ");
  Serial.print(meuPid.pid);
  Serial.print(" Sample: ");
  Serial.print(meuPid.sample);
  Serial.print(" ");
  Serial.print(" potVazao: ");
  Serial.print(potVazao);
  Serial.print(" ");
  Serial.print(" admin: ");
  Serial.print(adminMode);
  Serial.println();
  //
}
 
