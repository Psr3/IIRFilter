#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define REG_SIZE 3
#define PI 3.14
#define STAGES 4
#define SIZESIGNAL 20
#define THRESHOLDVALUE 20



/*IIR Filter second order */

void IIRFilter(long Signal, long NumCoeff[3], long DenCoeff[4][3], long gain[4], long *Reg){
    /*Shift register + Add signal to input Signal*/
    Reg[2]=Reg[1];
    Reg[1]=Reg[0];
    Reg[0]=Signal;
    
    /*First stage*/
    Reg[REG_SIZE+2]= Reg[REG_SIZE+1];
    Reg[REG_SIZE+1]= Reg[REG_SIZE+0];
    Reg[REG_SIZE+0]= ((NumCoeff[0]*Reg[0] + NumCoeff[1]*Reg[1] + NumCoeff[2]*Reg[2] - DenCoeff[0][1]*Reg[REG_SIZE+1] - DenCoeff[0][2]*Reg[REG_SIZE+2]))/100000 ;
    
    /*Second stage*/
    Reg[2*REG_SIZE+2]= Reg[2*REG_SIZE+1];
    Reg[2*REG_SIZE+1]= Reg[2*REG_SIZE+0];
    Reg[2*REG_SIZE+0]= (((NumCoeff[0]*Reg[REG_SIZE+0] + NumCoeff[1]*Reg[REG_SIZE+1] + NumCoeff[2]*Reg[REG_SIZE+2])*gain[0])/100000 - DenCoeff[1][1]*Reg[2*REG_SIZE+1] - DenCoeff[1][2]*Reg[2*REG_SIZE+2])/100000 ;
    
    /*Third stage*/
    Reg[3*REG_SIZE+2]= Reg[3*REG_SIZE+1];
    Reg[3*REG_SIZE+1]= Reg[3*REG_SIZE+0];
    Reg[3*REG_SIZE+0]= (((NumCoeff[0]*Reg[2*REG_SIZE+0] + NumCoeff[1]*Reg[2*REG_SIZE+1] + NumCoeff[2]*Reg[2*REG_SIZE+2])*gain[1])/100000 - DenCoeff[2][1]*Reg[3*REG_SIZE+1] - DenCoeff[2][2]*Reg[3*REG_SIZE+2])/100000 ;
    
    /*Fourth stage*/
    Reg[4*REG_SIZE+2]= Reg[4*REG_SIZE+1];
    Reg[4*REG_SIZE+1]= Reg[4*REG_SIZE+0];
    Reg[4*REG_SIZE+0]= (((NumCoeff[0]*Reg[3*REG_SIZE+0] + NumCoeff[1]*Reg[3*REG_SIZE+1] + NumCoeff[2]*Reg[3*REG_SIZE+2])*gain[2])/100000 - DenCoeff[3][1]*Reg[4*REG_SIZE+1] - DenCoeff[3][2]*Reg[4*REG_SIZE+2])/100000 ;


    
}

/*Return the maximum value of a list*/
long maxValue(long *FilteredSignal,int SizeSignal){
 int i;
 long max=FilteredSignal[0];
 for(i=0;i<SizeSignal;i++){
     long evaluatedValue=FilteredSignal[i];
     if(evaluatedValue<0){
         evaluatedValue=-evaluatedValue;
     }
     if(max<evaluatedValue){
         max=evaluatedValue;
         
     }
 }
 return max;
 }

/*Shift value in a list and after add the new value in first position*/
void addToFilteredSignal(long *FilteredSignal, long input,int SizeSignal){
    int k;
    for(k=SizeSignal-1; k>0; k--)FilteredSignal[k] = FilteredSignal[k-1];
    FilteredSignal[0]=input;
}

/*Section1 IIR filter 900Hz*/
long  filter900(long Signal,long *Reg900){
    long NumCoeff[3] = {100000, 0,-100000};
    long DenCoeff[4][3] = {{100000,-184923,   99430},{100000,-185935,  99449},{100000,-184491,   98644},{100000,-184919,   98663}};
    long Gain[4]={731, 731, 728, 728} ;
    //long Gain[4]={1, 1, 1, 1};
    IIRFilter(Signal, NumCoeff, DenCoeff, Gain,  Reg900 );
    long y=((Reg900[4*REG_SIZE+0])*Gain[3])/100000;
    /*printf("Valeur de y: %f\n",y);*/
    return y;
}
/* Section2 IIR filter 1100Hz*/
long  filter1100(long Signal,long *Reg1100){
    long NumCoeff[3] = {100000, 0,-100000};
    long DenCoeff[4][3] = {{100000,-177777,  99304},{100000,-179266,  99327 },{100000,-177360 ,   98345},{100000,-177985,   98368}};
    long Gain[4]={893, 893, 889, 889} ;
    //long Gain[4]={1, 1, 1, 1};
    IIRFilter(Signal, NumCoeff, DenCoeff, Gain,  Reg1100 );
    long y=((Reg1100[4*REG_SIZE+0])*Gain[3])/100000;
    /*printf("Valeur de y: %f\n",y);*/
    return y;
}





void run(){
    float Temps;
    int Periode=100;
    int frequence=1100;
    int count=0;
    float pas=0.000066667;
    // float pas = 1;
    /*printf("Valeur du pas: %f\n",pas);*/
    long * FilteredSignal900 = (long * )malloc( SIZESIGNAL*sizeof(long)); // Make a List of SIZESIGNAL values (SIZESIGNAL= number of values in one period) composed by filtered values
    long * FilteredSignal1100 = (long * )malloc( SIZESIGNAL*sizeof(long)); // Make a List of SIZESIGNAL values (SIZESIGNAL= number of values in one period) composed by filtered values
    long * Reg900 = (long *)malloc((STAGES+1)*REG_SIZE* sizeof(long)); // Make a Register List used to filter the Signal
    long * Reg1100 = (long *)malloc((STAGES+1)*REG_SIZE* sizeof(long)); // Make a Register List used to filter the Signal
    for(int i=0;i<15;i++){
        Reg900[i]=0;
        Reg1100[i]=0;
    }
    for(int i=0;i<SIZESIGNAL;i++){
        FilteredSignal900[i]=0;   //Define all list's values at 0
        FilteredSignal1100[i]=0;  //Define all list's values at 0
    }
    while(Temps<10){
        long Signal=(4*sin(2*PI*frequence*Temps))*100000; //Simulation of Sinusoidal signal
        long input900= filter900(Signal, Reg900); //Result of the 900Hz filter
        long input1100= filter1100(Signal, Reg1100); //Result of the 1100Hz filter
        addToFilteredSignal(FilteredSignal900, input900, SIZESIGNAL); //Add the 900Hz filtered value to the filtered Signal list
        addToFilteredSignal(FilteredSignal1100, input1100, SIZESIGNAL); //Add the 1100Hz filtered value to the filtered Signal list
        if(count==20){
            long maxValue900=maxValue(FilteredSignal900,SIZESIGNAL); //Maximum value of the Filtered Signal 900 on one Periode
            long maxValue1100=maxValue(FilteredSignal1100,SIZESIGNAL); //Maximum value of the Filtered Signal 1100 on one Periode
            if(THRESHOLDVALUE<maxValue900){ //compare the MaxValue900 with the THRESHOLDVALUE
                printf("\n C'est du 900");
            }
            if(THRESHOLDVALUE<maxValue1100){ //compare the MaxValue1100 with the THRESHOLDVALUE
                printf("\n C'est du 1100");
            }
            count=0;
        }
        Temps= Temps+pas;
        count= count+1;
    }
    free(FilteredSignal900);
    free(FilteredSignal1100);
    free(Reg900);
    free(Reg1100);
    
    
}


int main() {
    run();
    return 0;
}
