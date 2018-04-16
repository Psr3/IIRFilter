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

void IIRFilter(float Signal, float NumCoeff[3], float DenCoeff[4][3], float gain[4], float *Reg){
    /*Shift register + add signal to input Signals*/
    Reg[2]=Reg[1];
    Reg[1]=Reg[0];
    Reg[0]=Signal;
    
    /*First stage*/
    Reg[REG_SIZE+2]= Reg[REG_SIZE+1];
    Reg[REG_SIZE+1]= Reg[REG_SIZE+0];
    Reg[REG_SIZE+0]= NumCoeff[0]*Reg[0] + NumCoeff[1]*Reg[1] + NumCoeff[2]*Reg[2] - DenCoeff[0][1]*Reg[REG_SIZE+1] - DenCoeff[0][2]*Reg[REG_SIZE+2];
    
    /*Second stage*/
    Reg[2*REG_SIZE+2]= Reg[2*REG_SIZE+1];
    Reg[2*REG_SIZE+1]= Reg[2*REG_SIZE+0];
    Reg[2*REG_SIZE+0]= (NumCoeff[0]*Reg[REG_SIZE+0] + NumCoeff[1]*Reg[REG_SIZE+1] + NumCoeff[2]*Reg[REG_SIZE+2])*gain[0] - DenCoeff[1][1]*Reg[2*REG_SIZE+1] - DenCoeff[1][2]*Reg[2*REG_SIZE+2];
    
    /*Third stage*/
    Reg[3*REG_SIZE+2]= Reg[3*REG_SIZE+1];
    Reg[3*REG_SIZE+1]= Reg[3*REG_SIZE+0];
    Reg[3*REG_SIZE+0]= (NumCoeff[0]*Reg[2*REG_SIZE+0] + NumCoeff[1]*Reg[2*REG_SIZE+1] + NumCoeff[2]*Reg[2*REG_SIZE+2])*gain[1] - DenCoeff[2][1]*Reg[3*REG_SIZE+1] - DenCoeff[2][2]*Reg[3*REG_SIZE+2];
    
    /*Fourth stage*/
    Reg[4*REG_SIZE+2]= Reg[4*REG_SIZE+1];
    Reg[4*REG_SIZE+1]= Reg[4*REG_SIZE+0];
    Reg[4*REG_SIZE+0]= (NumCoeff[0]*Reg[3*REG_SIZE+0] + NumCoeff[1]*Reg[3*REG_SIZE+1] + NumCoeff[2]*Reg[3*REG_SIZE+2])*gain[2] - DenCoeff[3][1]*Reg[4*REG_SIZE+1] - DenCoeff[3][2]*Reg[4*REG_SIZE+2];
    
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

void addToFilteredSignal(float *FilteredSignal, float input,int SizeSignal){
    int k;
    for(k=SizeSignal-1; k>0; k--)FilteredSignal[k] = FilteredSignal[k-1];
    FilteredSignal[0]=input;
}

/*Section1 IIR filter 900*/
float  filter900(float Signal,float *Reg900){
    float NumCoeff[3] = {1, 0,-1};
    float DenCoeff[4][3] = {{1,-1.849232881995307886668911123706493526697,   0.994308932656938981864414017763920128345},{1,-1.859357338914342516744682143325917422771,   0.994499858380903267729422623233404010534},{1,-1.844916615723619868205673810734879225492,   0.986448999510018187386606314248638227582},{1,-1.849196871265132324779756345378700643778,   0.986638483540468680388357824995182454586}};
    float Gain[4]={0.007311183296068102070719429974587910692, 0.007311183296068102070719429974587910692, 0.007282505737740874347807551458799935062, 0.007282505737740874347807551458799935062} ;
    IIRFilter(Signal, NumCoeff, DenCoeff, Gain,  Reg900 );
    float y=(Reg900[4*REG_SIZE+0])*Gain[3];
    /*printf("Valeur de y: %f\n",y);*/
    return y;
}
/* Section2 IIR filter 1100*/

/*double *filter1100(double *Signal){
 double *FilteredSignal1100;
 double NumCoeff[] = {1, 0, -1};
 double DenCoeff1[] = {1, -1.509752573387514118508079263847321271896,  0.983275449259605083263124924997100606561};
 double DenCoeff2[] = {1,-1.546502728726402153824892593547701835632,  0.983841521223039250365616226190468296409};
 double DenCoeff3[] = {1,-1.515972573399827938445127983868587762117,  0.967378393534816805043874410330317914486};
 RunIIRPolyForm2( Signal, FilteredSignal1100, NumCoeff, DenCoeff1);
 RunIIRPolyForm2( FilteredSignal1100, FilteredSignal1100, NumCoeff, DenCoeff2);
 RunIIRPolyForm2( FilteredSignal1100, FilteredSignal1100, NumCoeff, DenCoeff3);
 return FilteredSignal1100;
 }*/


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
    int frequence=950;
    float pas=0.000066667;
    /*printf("Valeur du pas: %f\n",pas);*/
    float * FilteredSignal900 = (float * ) malloc( Periode*sizeof(float));
    float * Reg900 = (float *)malloc((STAGES+1)*REG_SIZE* sizeof(float*));
    while(Temps<100){
        float Signal=4*sin(2*PI*frequence*Temps);
        printf("Valeur du Signal: %f\n",Signal);
        float input900= filter900(Signal, Reg900);
        printf("Valeur du Signal filtré: %f\n",input900);
        addToFilteredSignal(FilteredSignal900, input900, SIZESIGNAL);
        Temps= Temps+pas;
    }
    free(FilteredSignal900);
    free(Reg900);


}


int main() {
    run();
    return 0;
}
