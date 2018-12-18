//Harley Ross and Dalton Altstaetter
//StepperMotorFSM

#define OUTPUT		//ports used for output
int switch1Flag = 0, switch2Flag = 0, switch3Flag = 0;

struct State {
	unsigned long out;
	unsigned long delay;
	const struct State* next[3];
};

typedef const struct State SM;
#define five	&FSM[0]
#define six		&FSM[1]
#define ten		&FSM[2]
#define nine	&FSM[3]

//order for next states are: clockwise, counterclockwise, stop
SM FSM[4] = {
	{5, 10, {six, nine, five}},
	{6, 10, {ten, five, six}},
	{10, 10, {nine, six, ten}},
	{9, 10, {five, ten, nine}}
};

int main(void) {
	SM *Pt;
	unsigned long input;
	int counter = 0, counterFlag = 0;
	
	//initialization stuff
	
	while(1) {
		OUTPUT = Pt->out;
		//delay(Pt->delay);
		
		//ClockWise
		if(((switch1Flag != 0) && (switch2Flag == 0) && (switch3Flag == 0)) 
			|| ((switch1Flag == 0) && (switch2Flag == 0) && (switch3Flag != 0))) {
				Pt = Pt->next[0];
				switch1Flag = 0;
				switch3Flag = 0;
		}
		else if((switch1Flag != 0) && (switch2Flag != 0) && (switch3Flag == 0) && (counterFlag == 0)) {
			Pt = Pt->next[0];
			switch1Flag = 0;
			switch2Flag = 0;
			counter++;
			
			//changes counter and counter flag
			if(counter == 7) {
				counter = 0;
				if(counterFlag == 0) {
					counterFlag = 1;
				}
				else {
					counterFlag = 0;
				}
			}
		}
		
		//CounterClockWise
		else if((switch1Flag == 0) && (switch2Flag != 0) && (switch3Flag == 0)) {
			Pt = Pt->next[1];
			switch2Flag = 0;
		}
		else if((switch1Flag != 0) && (switch2Flag != 0) && (switch3Flag == 0)) {
			Pt = Pt->next[1];
			switch1Flag = 0;
			switch2Flag = 0;
			counter++;
			
			//changes counter and counter flag
			if(counter == 7) {
				counter = 0;
				if(counterFlag == 0) {
					counterFlag = 1;
				}
				else {
					counterFlag = 0;
				}
			}
		}
		
		//Stop
		else {
			Pt = Pt->next[2];
			switch1Flag = 0;
			switch2Flag = 0;
			switch3Flag = 0;
			counter = 0;
		}
	}
}
			