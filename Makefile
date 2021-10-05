all:
	gcc -Wall -O1 latency.c -o latency.x -std=c99	
	./latency.x 100

	gcc -Wall -O1 latency_simd_add.c -o latency_simd_add.x -std=c99	
	./latency_simd_add.x 100

	gcc -Wall -O1 latency_simd_fma.c -o latency_simd_fma.x -std=c99	
	./latency_simd_fma.x 100

	gcc -Wall -O1 throughput_simd_add.c -o throughput_simd_add.x -std=c99	
	./throughput_simd_add.x 100

	gcc -Wall -O1 throughput_simd_fma.c -o throughput_simd_fma.x -std=c99	
	./throughput_simd_fma.x 100
	
	gcc -Wall -O2 kernel_driver.c kernel.c -o kernel.x -mavx -mfma -std=c99
	./kernel.x
	
clean:
	rm -rf *.x *~
