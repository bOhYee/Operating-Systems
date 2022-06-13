/*
	Illustrate the “Producer and Consumer” problem (with P producers and C consumers), and report with pseudo-code
	a possible implementation schema. Indicate and motivate the function of all the semaphores.
	Then, adapt the previous solution to the case of exactly 3 producers and only one consumer. Each producer must
	generate elements in a queue dedicated to each process, the consumer must consume elements ensuring a higher
	priority to the queue with the major number of stored elements.
	Suggestion: use counters to track the number of elements in each queue, or alternatively use a function (e.g., sem
	getvalue) that can return the value of a semaphore.
*/


// SOLUTION FOR 3 P AND 1 C
#define SIZE 10
sem_t empty_q1, full_q1;
sem_init(empty, FULL);
sem_init(full, 0);

sem_t empty_q2, full_q2;
sem_init(empty_q2, FULL);
sem_init(full_q2, 0);

sem_t empty_q3, full_q3;
sem_init(empty_q3, FULL);
sem_init(full_q3, 0);

int n1 = 0, head1 = 0, tail1 = 0, q1[SIZE];
int n2 = 0, head2 = 0, tail2 = 0, q2[SIZE];
int n3 = 0, head3 = 0, tail3 = 0, q3[SIZE];

sem_t busy;
sem_init(busy, 1);

void enqueue(int *buff, int *n, int *tail, int data_to_ins){
		buff[(*tail)] = data_to_ins;
		*tail = ((*tail)+1) % SIZE;

		(*n)++;
}

int dequeue(int *buff, int *n, int *head){
		int data;

		data = buff[*head];
		*head = ((*head)+1) % SIZE;
		(*n)--;
}

// PRODUCER
void P1(){
		while (true){
				sem_wait(empty_q1);
				sem_wait(busy);
				enqueue(q1, &n1, &tail1, 1);
				sem_post(full_q1);
				sem_post(busy);
		}
}

// PRODUCER 2
void P2(){
		while (true){
				sem_wait(empty_q2);
				sem_wait(busy);
				enqueue(q2, &n2, &tail2, 1);
				sem_post(full_q2);
				sem_post(busy);
		}
}

// PRODUCER 3
void P3(){
		while (true){
				sem_wait(empty_q3);
				sem_wait(busy);
				enqueue(q3, &n3, &tail3, 1);
				sem_post(full_q3);
				sem_post(busy);
		}
}

// CONSUMER
void consumer(){
		int queue;
		int data;

		while (true){
				sem_wait(full_q1);
				sem_wait(full_q2);
				sem_wait(full_q3);
				sem_wait(busy);

				queue = max(n1, n2, n3);
				switch (queue){
						case 1:
								data = dequeue(q1, &n1, &head1);
								break;
						case 2:
								data = dequeue(q2, &n2, &head2);
								break;
						case 3:
								data = dequeue(q3, &n3, &head3);
								break;
				}

				sem_post(busy);
				sem_post(empty_q1);
				sem_post(empty_q2);
				sem_post(empty_q3);

				printf("%d\n", data);
		}
}
