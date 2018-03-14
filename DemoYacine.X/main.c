#include <stdio.h>
#include <stdlib.h>

#define REG_SIZE 3


int N=3;
int sizeSignal = 100;
// On crée un pointeur sur int

// La fonction malloc inscrit dans notre pointeur l'adresse qui a été reservée.
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

float RunIIRPolyForm2(float Signal, float NumCoeff[3], float DenCoeff[3], float gain, int N){
    float output;
    int j, k;
    float RegX[REG_SIZE];
    float RegY[REG_SIZE];
    float SumPos;
    float SumNeg;
    float y;
    
    for(j=0; j <REG_SIZE; j++)RegX[j] = RegY[j]= 0;  // Init the delay registers.
    
    int shift = 0;
    while(shift < N){
        {
            // Shift the register values.
            for(k=N-1; k>0; k--)RegX[k] = RegX[k-1];
            for(k=N-1; k>0; k--)RegY[k] = RegY[k-1];
            
            // The numerator
            SumPos= SumNeg = 0.0;
            RegX[0] = Signal;
            for(k=0; k<N; k++)
            {
                SumPos += NumCoeff[k] * RegX[k];
                printf("SumPos %f\n", SumPos);
            }
            
            // The denominator
            for(k=0; k<N; k++)
            {
                SumNeg -= DenCoeff[k] * RegY[k];
                printf("SumNeg %f\n", SumNeg);
            }
            RegY[0]=SumPos + SumNeg;
            y = RegY[0];
            printf("y %f\n", y);
            shift++;
        }
        
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
float  filter900(float Signal,  int N){
    float NumCoeff[] = {1, 0,-1};
    float DenCoeff1[] = {1,-1.664414318134467052345826232340186834335,  0.986290434543632965613824126194231212139};
    float DenCoeff2[] = {1,-1.689825793226944483649276662617921829224,  0.986785118376035330634010733774630352855};
    float DenCoeff3[] = {1,-1.666131600275946089695366936211939901114,  0.97325187491204023793045507773058488965 };
    float Gain1=0.013463182178534767452493525752288405783 ;
    float Gain2=0.013463182178534767452493525752288405783 ;
    float Gain3=0.013374062543979908790348076763621065766 ;
    printf("Coeff %f\n", DenCoeff1[1]);
    printf("Signal %f\n", Signal);
    float OutputFirstStage = RunIIRPolyForm2(Signal,  NumCoeff, DenCoeff1, Gain1,  N);
    printf("OutputFirstStage %f\n", OutputFirstStage);
    float OutputSecondStage=RunIIRPolyForm2( OutputFirstStage,  NumCoeff, DenCoeff2, Gain2,  N);
    printf("OutputSecondStage %f\n", OutputSecondStage);
    float FinalOutput=RunIIRPolyForm2( OutputSecondStage, NumCoeff, DenCoeff3,Gain3, N);
    printf("FinalOutput %f\n", FinalOutput);
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
    int Periode=100;
    float * FilteredSignal900 = (float * ) malloc( Periode*sizeof(float));
    while(Temps<Periode){
        printf("At time %d\n", Temps);
        if(Temps==0){
            int i;
            for(i=0;i<Periode;i++){
                FilteredSignal900[i]=0;
                
            }
        }
        printf("avant filtre actif\n");
        float valueToAdd=filter900(900,N);
        printf("apres filtre actif\n");
        addToFilteredSignal(FilteredSignal900,valueToAdd,100);
        /*if(Temps==99){
         Temps=-1;
         
         }*/
        Temps++;
    }
    free(FilteredSignal900);
    
    
}


int main() {
    run();
    return 0;
}

