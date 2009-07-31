/* ECDLP Solver pollard's pho method
 * v0.1 implemented the original algorithm 4.3 
 * v0.2 changed Branches L to 32
 * v0.3 some codes optimized
 * v0.4 changed Floyd's cycleing finding algorithm to Brent's improved cycle algorithm,more faster 
 * v0.5 implemented Pohlig-Hellman algorithm,much more faster 
 * v0.6 fixed the pollard's pho method failded bug
 */

#include "dlpsolver.h"

//#pragma comment()
#pragma comment(lib,"ms32.lib")

big Pollard_Pho_EC_Compute(big order_of_basepoint, epoint* base_point, epoint* public_point)
{
	big big_branches;
	big big_c1,big_c2,big_d1,big_d2;
	big big_x,big_temp,big_log;
	big x,y,big_k,big_j,big_r;
	int j=0,i=0;
	big a[WALK_BRANCHES],b[WALK_BRANCHES];
	epoint* R[WALK_BRANCHES], *ept_X1, *ept_X2;
	BOOL bDone=FALSE;

	big_branches=mirvar(4);
	big_c1=mirvar(0);
	big_c2=mirvar(0);
	big_d1=mirvar(0);
	big_d2=mirvar(0);
	big_x=mirvar(0);
	big_temp=mirvar(0);
	big_log=mirvar(0);
	x=mirvar(0);
	y=mirvar(0);
	big_j=mirvar(0);
	big_k=mirvar(0);
	big_r=mirvar(1);

	irand(GetTickCount());
	for (j=0;j<WALK_BRANCHES;j++)
	{
		a[j]=mirvar(0);
		b[j]=mirvar(0);
		R[j]=epoint_init();
	}
	ept_X1=epoint_init();
	ept_X2=epoint_init();
	while (1)
	{
		for (j=0;j<WALK_BRANCHES;j++)
		{
			bigrand(order_of_basepoint,a[j]);
			bigrand(order_of_basepoint,b[j]);
			ecurve_mult2(a[j],base_point,b[j],public_point,R[j]);
		}
		bigrand(order_of_basepoint,big_c1);
		bigrand(order_of_basepoint,big_d1);
		ecurve_mult2(big_c1,base_point,big_d1,public_point,ept_X1);
		do 
		{
			copy(big_c1,big_c2);
			copy(big_d1,big_d2);
			epoint_copy(ept_X1,ept_X2);
			copy(big_k,big_j);
			add(big_r,big_r,big_r);
			do 
			{
				incr(big_k,1,big_k);
				epoint_get(ept_X1,big_x,big_x);
				j=remain(big_x,WALK_BRANCHES);
				ecurve_add(R[j],ept_X1);
				add(big_c1,a[j],big_c1);
				divide(big_c1,order_of_basepoint,order_of_basepoint);
				add(big_d1,b[j],big_d1);
				divide(big_d1,order_of_basepoint,order_of_basepoint);
				if (epoint_comp(ept_X1,ept_X2)==TRUE)
				{
					bDone=TRUE;
				}
			} while((bDone==FALSE)&&(compare(big_k,big_r)<0));
		} while(bDone==FALSE);
		if (compare(big_d2,big_d1)==0)
		{
			continue;
		} 
		else
		{
			divide(big_c1,order_of_basepoint,order_of_basepoint);
			divide(big_c2,order_of_basepoint,order_of_basepoint);
			subtract(big_d2,big_d1,big_temp);
			divide(big_temp,order_of_basepoint,order_of_basepoint);
			xgcd(big_temp,order_of_basepoint,big_temp,big_temp,big_temp);
			subtract(big_c1,big_c2,big_log);
			divide(big_log,order_of_basepoint,order_of_basepoint);
			multiply(big_log,big_temp,big_log);
			divide(big_log,order_of_basepoint,order_of_basepoint);
			if (size(big_log)<0)
			{
				add(big_log,order_of_basepoint,big_log);
			}
			ecurve_mult(big_log,base_point,ept_X1);
			if (epoint_comp(ept_X1,public_point)==TRUE)
			{
				//cotnum(big_log,stdout);
				break;
			} 
			else
			{
				zero(big_log);
				continue;
				//printf("Error... :(,please send your Elliptic Curve parameters to cnbragon@gmail.com\n");
			}
			//break;
		}
	}
	/*do 
	{
		epoint_get(ept_X1,big_x,big_x);
		j=remain(big_x,WALK_BRANCHES);
		ecurve_add(R[j],ept_X1);
		incr(x1[j],1,x1[j]);
		for (i=0;i<2;i++)
		{
			epoint_get(ept_X2,big_x,big_x);
			j=remain(big_x,WALK_BRANCHES);
			ecurve_add(R[j],ept_X2);
			incr(x2[j],1,x2[j]);
		}
#ifdef TEST
		epoint_get(ept_X1,x,y);
		printf("x1=(");
		cotnum(x,stdout);
		cotnum(y,stdout);
		printf(")\n");
		epoint_get(ept_X2,x,y);
		printf("x2=(");
		cotnum(x,stdout);
		cotnum(y,stdout);
		printf(")\n");
		cotnum(big_d1,stdout);
		cotnum(big_d2,stdout);
#endif
	} while(epoint_comp(ept_X1,ept_X2)==FALSE);*/
	
	mirkill(big_temp);
	mirkill(big_x);
	mirkill(big_branches);
	mirkill(big_c1);
	mirkill(big_c2);
	mirkill(big_d1);
	mirkill(big_d2);
	for (j=0;j<4;j++)
	{
		mirkill(a[j]);
		mirkill(b[j]);
		epoint_free(R[j]);
	}
	return big_log;
}
//#define TEST2
big Pohlig_Hellman_EC_Compute(PHE_Instance pheinst)
{
	int i=0,j=0;
	epoint *pt_Q0, *pt_Q1, *pt_P0, *pt_P1;
	big big_log,big_T0,big_T1,big_T2,big_order0;
	big big_a[MAX_PRIMES],big_x[MAX_PRIMES],big_moduli[MAX_PRIMES];
	big_chinese crt1;
	big x,y;

	printf("\t*******************Calculating Logarithm*******************\n");
	printf("\t*******************  Please be patient! *******************\n");
	pt_Q0=epoint_init();
	pt_Q1=epoint_init();
	pt_P0=epoint_init();
	pt_P1=epoint_init();

	big_T0=mirvar(0);
	big_T1=mirvar(0);
	big_T2=mirvar(0);
	big_order0=mirvar(0);
	x=mirvar(0);
	y=mirvar(0);
	
	copy(pheinst.Pohell_status.es_order,big_order0);
#ifdef TEST2
	printf("Order0 is:");
	cotnum(big_order0,stdout);
#endif
	for (i=0;i<pheinst.phe_nspf;i++)
	{
		j=0;
		big_x[i]=mirvar(0);
		epoint_copy(pheinst.Pohell_status.es_ptq,pt_Q0);
#ifdef TEST2
		epoint_get(pt_Q0,x,y);
		printf("x co-ordinate of Q0:");
		cotnum(x,stdout);
		printf("y co-ordinate of Q0:");
		cotnum(y,stdout);
#endif
		copy(big_order0,big_T0);
#ifdef TEST2
		printf("Sub order:");
		cotnum(pheinst.phe_spf[i],stdout);
#endif
		divide(big_T0,pheinst.phe_spf[i],big_T0);
#ifdef TEST2
		printf("T0 is:");
		cotnum(big_T0,stdout);
#endif
		ecurve_mult(big_T0,pheinst.Pohell_status.es_ptp,pt_P0);
		while (j<pheinst.phe_c[i])
		{
			power(pheinst.phe_spf[i],(j+1),big_T1,big_T1);
#ifdef TEST2
			printf("Sub Order's %d exp=",(j+1));
			cotnum(big_T1,stdout);
#endif
			copy(big_order0,big_T2);
			divide(big_T2,big_T1,big_T2);
#ifdef TEST2
			printf("T2 is:");
			cotnum(big_T2,stdout);
#endif
			ecurve_mult(big_T2,pt_Q0,pt_Q1);
			big_a[j]=mirvar(0);

			epoint_get(pt_P0,x,y);
			printf("x co-ordinate of P0:");
			cotnum(x,stdout);
			printf("y co-ordinate of P0:");
			cotnum(y,stdout);
			epoint_get(pt_Q1,x,y);
			printf("x co-ordinate of Q1:");
			cotnum(x,stdout);
			printf("y co-ordinate of Q1:");
			cotnum(y,stdout);

			big_a[j]=Pollard_Pho_EC_Compute(pheinst.phe_spf[i],pt_P0,pt_Q1);

			printf("this Logarithm is:");
			cotnum(big_a[j],stdout);

			power(pheinst.phe_spf[i],j,big_T1,big_T1);
			multiply(big_a[j],big_T1,big_T1);
			add(big_T1,big_x[i],big_x[i]);
			ecurve_mult(big_T1,pheinst.Pohell_status.es_ptp,pt_P1);
			ecurve_sub(pt_P1,pt_Q0);
			mirkill(big_a[j]);
			j++;
		}
	}

	printf("Appling CRT(Chinese Remainder Theorem)...\n");
	big_log=mirvar(0);
	for (i=0;i<pheinst.phe_nspf;i++)
	{
		power(pheinst.phe_spf[i],pheinst.phe_c[i],big_T0,big_T0);
		big_moduli[i]=mirvar(0);
		copy(big_T0,big_moduli[i]);
	}
	crt_init(&crt1,pheinst.phe_nspf,big_moduli);
	crt(&crt1,big_x,big_log);
	crt_end(&crt1);
	
	epoint_free(pt_Q0);
	epoint_free(pt_Q1);
	epoint_free(pt_P0);
	epoint_free(pt_P1);

	mirkill(big_T0);
	mirkill(big_T1);
	mirkill(big_T2);
	mirkill(big_order0);
	

	return big_log;
}

void Solver_ECDLP_Init(ECDLP_Instance* ecdlpinst)
{
	big big_a,big_b,big_p,big_order;
	char szEC[2*MAX_PRIMES];
	big big_px,big_py,big_qx,big_qy;
	epoint *ept_P, *ept_Q;
	int lsb=0;

	big_a=mirvar(0);
	big_b=mirvar(0);
	big_p=mirvar(0);
	printf("1.Initialize Elliptic Curve Over F(p):y^2=x^3+ax+b mod p\n");
	printf("Note:Input and Output Base is 16!\n");
	printf("a=");
	scanf("%s",&szEC);
	cinstr(big_a,szEC);
	memset(szEC,0,2*MAX_PRIMES);
	ecdlpinst->es_a=mirvar(0);
	copy(big_a,ecdlpinst->es_a);

	printf("b=");
	scanf("%s",&szEC);
	cinstr(big_b,szEC);
	memset(szEC,0,2*MAX_PRIMES);
	ecdlpinst->es_b=mirvar(0);
	copy(big_b,ecdlpinst->es_b);

	printf("p=");
	scanf("%s",&szEC);
	cinstr(big_p,szEC);
	memset(szEC,0,2*MAX_PRIMES);
	ecdlpinst->es_p=mirvar(0);
	copy(big_p,ecdlpinst->es_p);
#ifdef TEST
	cotnum(big_a,stdout);
	cotnum(big_b,stdout);
	cotnum(big_p,stdout);
#endif
	ecurve_init(big_a,big_b,big_p,MR_PROJECTIVE);

	big_px=mirvar(0);
	big_py=mirvar(0);
	printf("2.Coordinates of Base Point P\n");
	printf("x-coordinate:");
	scanf("%s",&szEC);
	cinstr(big_px,szEC);
	memset(szEC,0,2*MAX_PRIMES);
	printf("least signaficant bit of y-coordinate(0 or 1):");
	scanf("%d",&lsb);
	ept_P=epoint_init();
	epoint_set(big_px,big_px,lsb,ept_P);
	ecdlpinst->es_ptp=epoint_init();
	epoint_copy(ept_P,ecdlpinst->es_ptp);
#ifdef TEST
	epoint_get(ept_P,big_px,big_py);
	cotnum(big_py,stdout);
#endif
	mirkill(big_px);
	mirkill(big_py);
	epoint_free(ept_P);

	big_order=mirvar(0);
	printf("The Order of this Base Point:");
	scanf("%s",&szEC);
	cinstr(big_order,szEC);
	ecdlpinst->es_order=mirvar(0);
	copy(big_order,ecdlpinst->es_order);
	memset(szEC,0,2*MAX_PRIMES);
	mirkill(big_order);


	big_qx=mirvar(0);
	big_qy=mirvar(0);
	printf("3.Coordinates of Public Key Q\n");
	printf("x-coordinate:");
	scanf("%s",&szEC);
	cinstr(big_qx,szEC);
	memset(szEC,0,2*MAX_PRIMES);
	printf("least signaficant bit of y-coordinate(0 or 1):");
	scanf("%d",&lsb);
	ept_Q=epoint_init();
	epoint_set(big_qx,big_qx,lsb,ept_Q);
	ecdlpinst->es_ptq=epoint_init();
	epoint_copy(ept_Q,ecdlpinst->es_ptq);
#ifdef TEST
	epoint_get(ept_Q,big_qx,big_qy);
	cotnum(big_qy,stdout);
#endif
	mirkill(big_qx);
	mirkill(big_qy);
	epoint_free(ept_Q);
	mirkill(big_a);
	mirkill(big_b);
	mirkill(big_p);
}

void Pohlig_Hellman_EC_Init(ECDLP_Instance ecdlpinst, PHE_Instance* pheinst)
{
	
	big bg_spf[MAX_PRIMES]; /* stores the small prime factors */
	int nsf=2; /* number of small prime factors of Order p */
	int c[MAX_PRIMES]={0}; /* the exp of each prime factor */
	int i=0,j=0;
	char szBuffer[2*MAX_PRIMES]={0};
	big big_order,big_mid;

	pheinst->Pohell_status.es_a=mirvar(0);
	pheinst->Pohell_status.es_b=mirvar(0);
	pheinst->Pohell_status.es_p=mirvar(0);
	pheinst->Pohell_status.es_order=mirvar(0);
	pheinst->Pohell_status.es_ptp=epoint_init();
	pheinst->Pohell_status.es_ptq=epoint_init();
	copy(ecdlpinst.es_a,pheinst->Pohell_status.es_a);
	copy(ecdlpinst.es_b,pheinst->Pohell_status.es_b);
	copy(ecdlpinst.es_p,pheinst->Pohell_status.es_p);
	copy(ecdlpinst.es_order,pheinst->Pohell_status.es_order);
	epoint_copy(ecdlpinst.es_ptp,pheinst->Pohell_status.es_ptp);
	epoint_copy(ecdlpinst.es_ptq,pheinst->Pohell_status.es_ptq);

	big_order=mirvar(1);
	big_mid=mirvar(1);
	
	printf("4.Give me the number of small prime factors of Order p,should less than 256!\ni=");
	scanf("%d",&nsf);
	pheinst->phe_nspf=nsf;

	printf("5.Give me the small prime factors and there exponets\n");
	for (i=0;i<nsf;i++)
	{
		printf("Prime factor:\n");
		bg_spf[i]=mirvar(0);
		pheinst->phe_spf[i]=mirvar(0);
		scanf("%s",szBuffer);
		cinstr(bg_spf[i],szBuffer);
		copy(bg_spf[i],pheinst->phe_spf[i]);
		printf("\nits exponets:");
		scanf("%d",&c[i]);
		pheinst->phe_c[i]=c[i];
		//power(bg_spf[i],c[i],big_mid,big_mid);
		//multiply(big_mid,big_order,big_order);
	}
	//cotnum(big_order,stdout);

}

void Test_EC()
{
	big a,b,p,order,k,px,py,qx,qy;
	epoint* ep, *eq;

	a=mirvar(0);
	b=mirvar(0);
	p=mirvar(0);
	order=mirvar(0);
	k=mirvar(0);
	px=mirvar(0);
	py=mirvar(0);
	qx=mirvar(0);
	qy=mirvar(0);

	cinstr(a,"437823ED2154979A");
	cinstr(b,"AC83A5435504A4B2");
	divide(a,b,b);
	cotnum(a,stdout);
	cinstr(a,"DDC8");
	cinstr(b,"F787");
	cinstr(p,"AC83A544B5962573");
	ecurve_init(a,b,p,MR_PROJECTIVE);
	ep=epoint_init();
	eq=epoint_init();
	cinstr(px,"1E9E28A3F5293B8A");
	cinstr(py,"4F0AA3B383482867");
	epoint_set(px,py,1,ep);
	
	cinstr(k,"437823ED2154979A");
	ecurve_mult(k,ep,eq);
	epoint_get(eq,qx,qy);
	cotnum(qx,stdout);
	cotnum(qy,stdout);
	cinstr(order,"AC83A5435504A4B2");
	ecurve_mult(order,ep,eq);
	epoint_get(eq,qx,qy);
	cotnum(qx,stdout);
	cotnum(qy,stdout);
	if (point_at_infinity(eq)==TRUE)
	{
		printf("ok\n");
	}
	epoint_free(ep);
	epoint_free(eq);
	mirkill(a);
	mirkill(b);
	mirkill(p);
	mirkill(order);
	mirkill(k);
	mirkill(px);
	mirkill(py);
	mirkill(qx);
	mirkill(qy);
}

void Usage()
{
	printf("\t***************  DLP&ECDLP Solver v0.6  *****************\n");
	printf("\t**********  Coded by Cnbragon[iPB][RCT][TT]  ************\n\n");
}

void main()
{
	miracl* mip=mirsys(800,16);
	ECDLP_Instance ecdlp1;
	PHE_Instance phe1;
	BOOL bPrime=FALSE;
	big logarithm;
	//int i;

	Usage();
	mip->IOBASE=16;

	//Test_EC();
	logarithm=mirvar(0);
	Solver_ECDLP_Init(&ecdlp1);

	if (isprime(ecdlp1.es_order)==TRUE)
	{
		printf("\t*******************Calculating Logarithm*******************\n");
		printf("\t*******************  Please be patient! *******************\n");
		logarithm=Pollard_Pho_EC_Compute(ecdlp1.es_order,ecdlp1.es_ptp,ecdlp1.es_ptq);
	} 
	else
	{
		Pohlig_Hellman_EC_Init(ecdlp1,&phe1);
		logarithm=Pohlig_Hellman_EC_Compute(phe1);
	}
	printf("\t*******************Successfully Calculated Logarithm*******************\n");
	printf("The Logarithm is :\n");
	cotnum(logarithm,stdout);
	mirkill(logarithm);
	//scanf("%d",&i);
}
