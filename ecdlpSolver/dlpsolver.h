#ifndef DLP_SOLVER_H
#define DLP_SOLVER_H

#include <stdlib.h>
#include <stdio.h>
#include <miracl/miracl.h>
#include <windows.h>

//#define TEST
#define WALK_BRANCHES 32
#define MAX_PRIMES    256

typedef struct ECDLP_Solver
{
	big es_a;
	big es_b;
	big es_p;
	big es_order;
	epoint* es_ptp;
	epoint* es_ptq;
}ECDLP_Instance;

typedef struct Pohlig_Hellman_EC
{
	ECDLP_Instance Pohell_status;
	big phe_spf[MAX_PRIMES];            /* small prime factors */
	int phe_nspf;                       /* number of small prime factors */
	int phe_c[MAX_PRIMES];              /* exponents of each factor */
}PHE_Instance;

void Solver_ECDLP_Init(ECDLP_Instance* ecdlpinst);
void Pohlig_Hellman_EC_Init(ECDLP_Instance ecdlpinst, PHE_Instance* pheinst);
big Pollard_Pho_EC_Compute(big order_of_basepoint, epoint* base_point, epoint* public_point);
big Pohlig_Hellman_EC_Compute(PHE_Instance pheinst);

#endif