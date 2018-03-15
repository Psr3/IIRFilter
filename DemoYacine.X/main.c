#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define REG_SIZE 3
#define PI 3.14
#define STAGES 4


int sizeSignal = 100;
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

float RunIIRPolyForm2(float Signal, int stage, float NumCoeff[3], float DenCoeff[3], float gain, float **RegX, float **RegY){
    float output;
    int k;
    int N=3;
    float SumPos;
    float SumNeg;
    float y;
    int shift = 0;
    while(shift < N){
            // Shift the register values.
            for(k=N-1; k>0; k--)RegX[stage-1][k] = RegX[stage-1][k-1];
            for(k=N-1; k>0; k--)RegY[stage-1][k] = RegY[stage-1][k-1];

            // The numerator
            SumPos= SumNeg = 0.0;
            RegX[stage-1][0] = Signal;
            for(k=0; k<N; k++)
            {
                SumPos += NumCoeff[k] * RegX[stage-1][k];
                printf("SumPos %f\n", SumPos);
            }

            // The denominator
            for(k=0; k<N; k++)
            {
                SumNeg -= DenCoeff[k] * RegY[stage-1][k];
                printf("SumNeg %f\n", SumNeg);
            }
            RegY[stage-1][0]=SumPos + SumNeg;
            y = RegY[stage-1][0];
            printf("y %f\n", y);
            shift++;
    }

    output=y*gain;
    printf("output %f\n", output);
    return output;

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
    printf("%f\n", FilteredSignal[0] );
}

/*Section1 IIR filter 900*/
float  filter900(float Signal,float **RegX, float **RegY){
    float NumCoeff[] = {1, 0,-1};
    float DenCoeff1[] = {1,-1.849232881995307886668911123706493526697,   0.994308932656938981864414017763920128345};
    float DenCoeff2[] = {1,-1.859357338914342516744682143325917422771,   0.994499858380903267729422623233404010534};
    float DenCoeff3[] = {1,-1.844916615723619868205673810734879225492,   0.986448999510018187386606314248638227582};
    float DenCoeff4[] = {1,-1.849196871265132324779756345378700643778,   0.986638483540468680388357824995182454586};
    float Gain1=0.007311183296068102070719429974587910692 ;
    float Gain2=0.007311183296068102070719429974587910692 ;
    float Gain3=0.007282505737740874347807551458799935062 ;
    float Gain4=0.007282505737740874347807551458799935062  ;
    printf("Coeff %f\n", DenCoeff1[1]);
    printf("Signal %f\n", Signal);
    float OutputFirstStage = RunIIRPolyForm2(Signal,1,  NumCoeff, DenCoeff1, Gain1, RegX, RegY);
    printf("OutputFirstStage %f\n", OutputFirstStage);
    float OutputSecondStage=RunIIRPolyForm2( OutputFirstStage,2,  NumCoeff, DenCoeff2, Gain2, RegX, RegY);
    printf("OutputSecondStage %f\n", OutputSecondStage);
    float OutputThirdStage=RunIIRPolyForm2( OutputSecondStage,3, NumCoeff, DenCoeff3,Gain3, RegX, RegY);
    printf("FinalOutput %f\n", OutputThirdStage);
    float FinalOutput=RunIIRPolyForm2( OutputThirdStage,4, NumCoeff, DenCoeff4,Gain4, RegX, RegY);
    return FinalOutput;
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
    int Temps=0;
    int j, i;
    int Periode=100;
    int stage;
    float * FilteredSignal900 = (float * ) malloc( Periode*sizeof(float));
    float ** RegX900 = (float **)malloc(STAGES * sizeof(float*));;
    for(i = 0; i < STAGES; i++) RegX900[i] = (float *)malloc(REG_SIZE * sizeof(float));
    float ** RegY900 = (float **)malloc(STAGES * sizeof(float*));;
    for(i = 0; i < STAGES; i++) RegY900[i] = (float *)malloc(REG_SIZE * sizeof(float));

    for (stage=0;stage<STAGES;stage++){
        for(j=0; j <REG_SIZE; j++){
            RegX900[stage][j]  = RegY900[stage][j] = 0.0;
        }
    }  // Init the delay registers.
    for (stage=0;stage<STAGES;stage++){
        for(j=0; j <REG_SIZE; j++){
            printf("%f\n", RegX900[stage][j]);
        }
    }  // Init the delay registers.

    float Signal;
    float Frequence=900;
    while(Temps<Periode){
        printf("At time %d\n", Temps);
        if(Temps==0){
            int i;
            for(i=0;i<Periode;i++){
                FilteredSignal900[i]=0;

            }
        }
        Signal= 4*sin(2*PI*Frequence*Temps);
        float valueToAdd=filter900(Signal, RegX900, RegY900);
        printf("apres filtre actif\n");
        addToFilteredSignal(FilteredSignal900,valueToAdd,100);
        /*if(Temps==99){
         Temps=-1;

     }*/
        Temps++;
    }
    free(FilteredSignal900);
    free(RegX900);
    free(RegY900);


}


int main() {
    run();
    return 0;
}
