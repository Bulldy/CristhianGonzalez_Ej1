placas.pdf: grafica.py v0.txt
	python grafica.py
v0.txt: a.out submit_job.sh
	qsub submit_job.sh
a.out: placas_p.c
	mpicc placas_p.c
clean:
	rm -f a.out v0.txt v1.txt v2.txt v3.txt ex0.txt ex1.txt ex2.txt ex3.txt ey0.txt ey1.txt ey2.txt ey3.txt