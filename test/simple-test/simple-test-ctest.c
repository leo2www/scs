/*
 * 一个简单的 SCS 调用示例
 * 求解线性规划: minimize c'x, s.t. Ax + s = b, s >= 0
 */
#include <stdio.h>
#include "scs.h"


#include "glbopts.h"

int main(void) {
	//min x1+x2
	//s.t.x1 + 3x2 >= 2 -s in l
	//x1,x2>=0, -s in l
	ScsCone* k = (ScsCone*)scs_calloc(1, sizeof(ScsCone));
	ScsData* d = (ScsData*)scs_calloc(1, sizeof(ScsData));
	
	d->m = 3; //约束数量
	d->n = 2; //变量数量

	// A in Comporessed Sparse Column (CSC) format
	scs_float A_x[4] = { -1.0, -1.0, -3.0, -1.0 };		// 非零元素 值
	scs_int A_i[4] = { 0, 1, 0, 2};					// 非零元素 行索引
	scs_int A_p[3] = { 0, 2 ,4};						// 列指针
	d->A = &(ScsMatrix) { A_x, A_i, A_p, d->m, d->n };
	// P is NULL for LPs
	d->P = SCS_NULL;
	// b vector
	d->b = (scs_float[3]){ -2.0,0.0, 0.0};
	// c vector
	d->c = (scs_float[]){ 1.0, 1.0 };

	// cone
	k->l = 3; // 非负正交锥的数量

	// settings
	ScsSettings *stgs = (ScsSettings *)scs_calloc(1,sizeof(ScsSettings));
	scs_set_default_settings(stgs);
	stgs->eps_abs = 1e-10;
	stgs->eps_rel = 1e-10;
	stgs->verbose = 1;
	stgs->max_iters = 10000;

	// initialize SCS
	ScsWork* work = scs_init(d, k, stgs);

	ScsSolution* sol = (ScsSolution*)scs_calloc(1, sizeof(ScsSolution));
	ScsInfo* info = (ScsInfo*)scs_calloc(1, sizeof(ScsInfo));

	int exit_flag = scs_solve(work, sol	, info, 0);

	// free scss workspace
	scs_finish(work);

	/* Verify that SCS solved the problem */
	printf("SCS solved successfully: %i\n", exit_flag == SCS_SOLVED);

	/* Print some info about the solve */
	printf("SCS took %i iters, using the %s linear solver.\n", info->iter,
		info->lin_sys_solver);

	/* Print solution x */
	printf("Optimal solution vector x*:\n");
	for (int i = 0; i < d->n; ++i) {
		printf("x[%i] = %.10f\n", i, sol->x[i]);
	}

	/* Print dual solution y */
	printf("Optimal dual vector y*:\n");
	for (int i = 0; i < d->m; ++i) {
		printf("y[%i] = %.10f\n", i, sol->y[i]);
	}


	scs_free(k);
	scs_free(d);
	scs_free(stgs);
	scs_free(info);

	scs_free(sol->x);
	scs_free(sol->y);
	scs_free(sol->s);
	scs_free(sol);
	return 0;
}