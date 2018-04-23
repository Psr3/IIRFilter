#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define REG_SIZE 3
#define PI 3.14
#define STAGES 4
#define SIZESIGNAL 100

// On cr�e un pointeur sur int

// La fonction malloc inscrit dans notre pointeur l'adresse qui a �t� reserv�e.
/*double FilteredSignal1100[];*/


/*int random_number(int min_num, int max_num)
 {
 int result = 0, low_num = 0, hi_num = 0;
 if (min_num < max_num)
 {
 low_num = min_num;
 hi_num = max_num + 1; // include max_num in output
 } else {
 low_num = max_num + 1; // include max_num in output
 hi_num = min_num;
 }
 srand(time(NULL));
 result = (rand() % (hi_num - low_num)) + low_num;
 return result;
 }*/


/*Filter deuxieme ordre IIR*/

void IIRFilter(long Signal, long NumCoeff[3], long DenCoeff[4][3], long gain[4], long *Reg){
    /*Shift register + add signal to input Signals*/
    Reg[2]=Reg[1];
    Reg[1]=Reg[0];
    Reg[0]=Signal;
    
    /*First stage*/
    Reg[REG_SIZE+2]= Reg[REG_SIZE+1];
    Reg[REG_SIZE+1]= Reg[REG_SIZE+0];
    Reg[REG_SIZE+0]= ((NumCoeff[0]*Reg[0] + NumCoeff[1]*Reg[1] + NumCoeff[2]*Reg[2] - DenCoeff[0][1]*Reg[REG_SIZE+1] - DenCoeff[0][2]*Reg[REG_SIZE+2]))/100000 ;
    printf("Valeur REG1: %ld\n",Reg[REG_SIZE+0]);
    
    /*Second stage*/
    Reg[2*REG_SIZE+2]= Reg[2*REG_SIZE+1];
    Reg[2*REG_SIZE+1]= Reg[2*REG_SIZE+0];
    Reg[2*REG_SIZE+0]= (((NumCoeff[0]*Reg[REG_SIZE+0] + NumCoeff[1]*Reg[REG_SIZE+1] + NumCoeff[2]*Reg[REG_SIZE+2])*gain[0])/100000 - DenCoeff[1][1]*Reg[2*REG_SIZE+1] - DenCoeff[1][2]*Reg[2*REG_SIZE+2])/100000 ;
    printf("Valeur REG2: %ld\n",Reg[2*REG_SIZE+0]);
    
    /*Third stage*/
    Reg[3*REG_SIZE+2]= Reg[3*REG_SIZE+1];
    Reg[3*REG_SIZE+1]= Reg[3*REG_SIZE+0];
    Reg[3*REG_SIZE+0]= (((NumCoeff[0]*Reg[2*REG_SIZE+0] + NumCoeff[1]*Reg[2*REG_SIZE+1] + NumCoeff[2]*Reg[2*REG_SIZE+2])*gain[1])/100000 - DenCoeff[2][1]*Reg[3*REG_SIZE+1] - DenCoeff[2][2]*Reg[3*REG_SIZE+2])/100000 ;
    printf("Valeur REG3: %ld\n",Reg[3*REG_SIZE+0]);
    
    /*Fourth stage*/
    Reg[4*REG_SIZE+2]= Reg[4*REG_SIZE+1];
    Reg[4*REG_SIZE+1]= Reg[4*REG_SIZE+0];
    Reg[4*REG_SIZE+0]= (((NumCoeff[0]*Reg[3*REG_SIZE+0] + NumCoeff[1]*Reg[3*REG_SIZE+1] + NumCoeff[2]*Reg[3*REG_SIZE+2])*gain[2])/100000 - DenCoeff[3][1]*Reg[4*REG_SIZE+1] - DenCoeff[3][2]*Reg[4*REG_SIZE+2])/100000 ;
    printf("Valeur REG4: %ld\n",Reg[4*REG_SIZE+0]);
    /*
     int i;
     for (i = 0; i < 15; i++){
     printf("(long) Reg-%d = %ld \n", i, Reg[i]);
     }
     */
    
}

/*float maxValue(float *FilteredSignal,int SizeSignal){
 int i;
 float max=FilteredSignal[0];
 for(i=0;i<SizeSignal;i++){
 *      if(max<FilteredSignal[i]){
 max=FilteredSignal[i];
 *      }
 }
 return max;
 }
 */

void addToFilteredSignal(long *FilteredSignal, long input,int SizeSignal){
    int k;
    for(k=SizeSignal-1; k>0; k--)FilteredSignal[k] = FilteredSignal[k-1];
    FilteredSignal[0]=input;
}

/*Section1 IIR filter 900*/
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
/* Section2 IIR filter 1100*/
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




/*void comparing(float Signal,int SizeSignal){
 float MeanValue;
 float * FilteredSignal900= (float*) malloc(100*sizeof(float));
 float valueToAdd=filter900(Signal,N);
 addToFilteredSignal(FilteredSignal900,valueToAdd,SizeSignal);
 MeanValue=maxValue(FilteredSignal900,SizeSignal);
 free(FilteredSignal900);
 }
 */

void run(){
    double Temps;
    int Periode=100;
    int frequence=1100;
    float pas=0.000066667;
    // float pas = 1;
    /*printf("Valeur du pas: %f\n",pas);*/
    long * FilteredSignal900 = (long * ) malloc( Periode*sizeof(long));
    long * Reg900 = (long *)malloc((STAGES+1)*REG_SIZE* sizeof(long));
    long * Reg1100 = (long *)malloc((STAGES+1)*REG_SIZE* sizeof(long));
    for(int i=0;i<15;i++){
        Reg900[i]=0;
        Reg1100[i]=0;
    }
    while(Temps<10){
        long Signal=(4*sin(2*PI*frequence*Temps))*100000;
        printf("Valeur du Signal: %ld\n",Signal);
        /*long input900= filter900(Signal, Reg900);
        printf("Valeur du Signal filtré: %ld\n",input900);*/
        long input1100= filter1100(Signal, Reg1100);
        printf("Valeur du Signal filtré: %ld\n",input1100);
        /*addToFilteredSignal(FilteredSignal900, input900, SIZESIGNAL);*/
        Temps= Temps+pas;
    }
    free(FilteredSignal900);
    free(Reg900);
    
    
}


int main() {
    run();
    return 0;
}
