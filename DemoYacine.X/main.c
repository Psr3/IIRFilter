#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define REG_SIZE 3
#define PI 3.14
#define STAGES 4
#define SIZESIGNAL 20
#define SAMPLES 1500
#define THRESHOLDVALUE 1000
#define PERSAMPLES 15
#define NBSAMPLES (SAMPLES/PERSAMPLES)

/* DEFINITIONS DES LISTES ET VARIABLES */

volatile int detLOWlist[NBSAMPLES];
volatile int detHIGHlist[NBSAMPLES];
volatile int detLOW = 0;
volatile int detHIGH = 0;
volatile int count = 1;
volatile int indx = 0;

/*IIR Filter second order */
void IIRFilter(long Signal, long NumCoeff[3], long DenCoeff[4][3], long gain[4], long Reg[(STAGES+1)*REG_SIZE]){
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
long maxValue(long FilteredSignal[SIZESIGNAL]){
    int i;
    long max=FilteredSignal[0];
    for(i=0;i<SIZESIGNAL;i++){
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
void addToFilteredSignal(long FilteredSignal[SIZESIGNAL], long input){
    int k;
    for(k=SIZESIGNAL-1; k>0; k--)FilteredSignal[k] = FilteredSignal[k-1];
    FilteredSignal[0]=input;
}

/*Section1 IIR filter 900Hz*/
long  filter900(long Signal,long Reg900[(STAGES+1)*REG_SIZE]){
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
long  filter1100(long Signal,long Reg1100[(STAGES+1)*REG_SIZE]){
    long NumCoeff[3] = {100000, 0,-100000};
    long DenCoeff[4][3] = {{100000,-177777,  99304},{100000,-179266,  99327 },{100000,-177360 ,   98345},{100000,-177985,   98368}};
    long Gain[4]={893, 893, 889, 889} ;
    //long Gain[4]={1, 1, 1, 1};
    IIRFilter(Signal, NumCoeff, DenCoeff, Gain,  Reg1100 );
    long y=((Reg1100[4*REG_SIZE+0])*Gain[3])/100000;
    /*printf("Valeur de y: %f\n",y);*/
    return y;
}
/*
 long meanValue(long list[sizeList],int sizeList){
 int i;
 long meanValue=0;
 for (i=0;i<sizeList;i++){
 meanValue = meanValue+list[i];
 }
 meanValue = meanValue/sizeList;
 
 return meanValue;
 }
 */


void filtering(long Signal,long FilteredSignal900[SIZESIGNAL],long FilteredSignal1100[SIZESIGNAL],long Reg900[(STAGES+1)*REG_SIZE],long Reg1100[(STAGES+1)*REG_SIZE]){
    long input900= filter900(Signal, Reg900); //Result of the 900Hz filter
    /*printf("Valeur du filter900: %ld\n",input900);*/
    long input1100= filter1100(Signal, Reg1100); //Result of the 1100Hz filter
    /*printf("Valeur du filter1100: %ld\n",input1100);*/
    addToFilteredSignal(FilteredSignal900, input900); //Add the 900Hz filtered value to the filtered Signal list
    addToFilteredSignal(FilteredSignal1100, input1100); //Add the 1100Hz filtered value to the filtered Signal list
    if (count == 15){
        
        indx++;
        count = count+1;
    }else{
        count ++;
    }
}

void comparing(long FilteredSignal900[SIZESIGNAL],long FilteredSignal1100[SIZESIGNAL],int indx){
    long maxValue900=maxValue(FilteredSignal900); //Maximum value of the Filtered Signal 900 on one Periode
    long maxValue1100=maxValue(FilteredSignal1100); //Maximum value of the Filtered Signal 1100 on one Periode
    if(THRESHOLDVALUE<maxValue900){ //compare the MaxValue900 with the THRESHOLDVALUE
        detLOWlist[indx] = 1;
        printf("detLow: %d\n",detLOWlist[indx]);
    }
    if(THRESHOLDVALUE>=maxValue900){
        detLOWlist[indx] = 0;
        printf("detLow: %d\n",detLOWlist[indx]);
    }
    
    if(THRESHOLDVALUE<maxValue1100){ //compare the MaxValue1100 with the THRESHOLDVALUE
        detHIGHlist[indx] = 1;
        printf("detHigh: %d\n",detHIGHlist[indx]);
    }
    if(THRESHOLDVALUE>=maxValue1100){
        detHIGHlist[indx] = 0;
        printf("detHigh: %d\n",detHIGHlist[indx]);
    }
}





void runFilter(){
    double Temps;
    int Periode=100;
    int frequence=1100;
    float pas=0.000066667;
    int i;
    int j;
    int k;
    int count=0;
    int indx=0;
    long FilteredSignal900[SIZESIGNAL];
    long FilteredSignal1100[SIZESIGNAL];
    long Reg900[(STAGES+1)*REG_SIZE];
    long Reg1100[(STAGES+1)*REG_SIZE];
    int detLOWlist[NBSAMPLES];
    int detHIGHlist[NBSAMPLES];
    
    for(i=0;i<15;i++){
        Reg900[i]=0;
        Reg1100[i]=0;
    }
    for(j=0;j<SIZESIGNAL;j++){
        FilteredSignal900[j]=0;   //Define all list's values at 0
        FilteredSignal1100[j]=0;  //Define all list's values at 0
    }
    for(k=0;k<NBSAMPLES;k++){
        detLOWlist[k]=0;   //Define all list's values at 0
        detHIGHlist[k]=0;  //Define all list's values at 0
    }
    while(Temps<10){
        float Signal=4*sin(2*PI*frequence*Temps)*100000;
        /*printf("Valeur du Signal: %f\n",Signal);*/
        filtering(Signal,FilteredSignal900,FilteredSignal1100,Reg900,Reg1100);
        if(count%15==0){
            comparing(FilteredSignal900,FilteredSignal1100,indx);
            indx=indx+1;
            printf("Valeur de l'indice: %i\n",indx);
        }
        count=count+1;
        Temps=Temps+pas;
    }
    
}


int main() {
    runFilter();
    return 0;
}
