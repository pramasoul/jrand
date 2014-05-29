/* Goertzel Algorithm
 * http://cms.edn.com/uploads/SourceCode/09banks.txt
 */

#define PI 3.141592653589793

#include <stdio.h>
#include <math.h>

#define FLOATING	float
#define SAMPLE	unsigned char

#define SAMPLING_RATE	8000.0	//8kHz
#define TARGET_FREQUENCY	941.0	//941 Hz
#define N	205	//Block size

FLOATING coeff;
FLOATING Q1;
FLOATING Q2;
FLOATING sine;
FLOATING cosine;

SAMPLE testData[N];

/* Call this routine before every "block" (size=N) of samples. */
void ResetGoertzel(void)
{
  Q2 = 0;
  Q1 = 0;
}

/* Call this once, to precompute the constants. */
void InitGoertzel(void)
{
  int	k;
  FLOATING	floatN;
  FLOATING	omega;

  floatN = (FLOATING) N;
  k = (int) (0.5 + ((floatN * TARGET_FREQUENCY) / SAMPLING_RATE));
  omega = (2.0 * PI * k) / floatN;
  sine = sin(omega);
  cosine = cos(omega);
  coeff = 2.0 * cosine;

  printf("For SAMPLING_RATE = %f", SAMPLING_RATE);
  printf(" N = %d", N);
  printf(" and FREQUENCY = %f,\n", TARGET_FREQUENCY);
  printf("k = %d and coeff = %f\n\n", k, coeff);

  ResetGoertzel();
}

/* Call this routine for every sample. */
void ProcessSample(SAMPLE sample)
{
  FLOATING Q0;
  Q0 = coeff * Q1 - Q2 + (FLOATING) sample;
  Q2 = Q1;
  Q1 = Q0;
}


/* Basic Goertzel */
/* Call this routine after every block to get the complex result. */
void GetRealImag(FLOATING *realPart, FLOATING *imagPart)
{
  *realPart = (Q1 - Q2 * cosine);
  *imagPart = (Q2 * sine);
}

/* Optimized Goertzel */
/* Call this after every block to get the RELATIVE magnitude squared. */
FLOATING GetMagnitudeSquared(void)
{
  FLOATING result;

  result = Q1 * Q1 + Q2 * Q2 - Q1 * Q2 * coeff;
  return result;
}

/*** End of Goertzel-specific code, the remainder is test code. */

/* Synthesize some test data at a given frequency. */
void Generate(FLOATING frequency)
{
  int	index;
  FLOATING	step;

  step = frequency * ((2.0 * PI) / SAMPLING_RATE);

  /* Generate the test data */
  for (index = 0; index < N; index++)
  {
    testData[index] = (SAMPLE) (100.0 * sin(index * step) + 100.0);
  }
}

/* Demo 1 */
void GenerateAndTest(FLOATING frequency)
{
  int	index;

  FLOATING	magnitudeSquared;
  FLOATING	magnitude;
  FLOATING	real;
  FLOATING	imag;

  printf("For test frequency %f:\n", frequency);
  Generate(frequency);

  /* Process the samples */
  for (index = 0; index < N; index++)
  {
    ProcessSample(testData[index]);
  }

  /* Do the "basic Goertzel" processing. */
  GetRealImag(&real, &imag);

  printf("real = %f imag = %f\n", real, imag);

  magnitudeSquared = real*real + imag*imag;
  printf("Relative magnitude squared = %f\n", magnitudeSquared);
  magnitude = sqrt(magnitudeSquared);
  printf("Relative magnitude = %f\n", magnitude);

  /* Do the "optimized Goertzel" processing */
  magnitudeSquared = GetMagnitudeSquared();
  printf("Relative magnitude squared = %f\n", magnitudeSquared);
  magnitude = sqrt(magnitudeSquared);
  printf("Relative magnitude = %f\n\n", magnitude);

  ResetGoertzel();
}

/* Demo 2 */
void GenerateAndTest2(FLOATING frequency)
{
  int	index;

  FLOATING	magnitudeSquared;
  FLOATING	magnitude;
  FLOATING	real;
  FLOATING	imag;

  printf("Freq=%7.1f   ", frequency);
  Generate(frequency);

  /* Process the samples. */
  for (index = 0; index < N; index++)
  {
    ProcessSample(testData[index]);
  }

  /* Do the "standard Goertzel" processing. */
  GetRealImag(&real, &imag);

  magnitudeSquared = real*real + imag*imag;
  printf("rel mag^2=%16.5f   ", magnitudeSquared);
  magnitude = sqrt(magnitudeSquared);
  printf("rel mag=%12.5f\n", magnitude);

  ResetGoertzel();
}

int main(void)
{
  FLOATING freq;

  InitGoertzel();

  /* Demo 1 */
  GenerateAndTest(TARGET_FREQUENCY - 250);
  GenerateAndTest(TARGET_FREQUENCY);
  GenerateAndTest(TARGET_FREQUENCY + 250);

  /* Demo 2 */
  for (freq = TARGET_FREQUENCY - 300; freq <= TARGET_FREQUENCY + 300; freq += 15)
  {
    GenerateAndTest2(freq);
  }

  return 0;
}
