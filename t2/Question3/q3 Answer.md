1 It creates 3 threads.



2 

1) output1:

0
3
6

When thread1 aquirces v1's lock and v2's lock, v2 haven't acquired both of v2's lock and v3's lock and thus v2's value remains the same as when it was first initialized. v1 = v1 + v2 = (0, 1, 2) + (0, 2, 4) = (0, 3, 6).

2) output2:

0
7
14

Thread3 first acquires its corresponding 2 locks and changes: v3 = v3 + v1 = (0,3,6) + (0,1,2)   = (0,4,8). 

Then v2 = v2 + v3 =  (0,2,4) + (0,4,8) = (0,6,12).  

Then v1 = v1 + v2 = (0, 1, 2) + (0, 6, 12) = (0, 7, 14).



3 

The deadlock case:

When all threads all stuck in between "pthread_mutex_lock(&v_dst->lock)" and "pthread_mutex_lock(&v_src->lock)" , thread1 is waiting for v2's lock while holding v1's lock, thread2 is waiting for v3's lock while holding v2's lock, and thread3 is waiting for v1's lock while holding v3's lock.

Resource graph:

Please refer to the t2/Question3/question3-3-resource graph of deadlock.jpg

The resource graph proofs that there is a deadlock because each lock in the graph only have a single instance and there is a cycle in the graph.



4

It won't change.

1) swap lock: it does not avoid the deadlock because doing so will make all the arrows in the previous resource graph change their direction, and the cycle still exists.

2) swap unlock: it does not avoid the deadlock because there already exists the deadlock because of the 2 lock statement.



5

Please refer to the t2/Question3/right.cpp to see my proposed change.

Add vector_add_for_v3(), change the order of invoking pthread_mutex_lock(): first lock v1 and then lock v3.

Correctness: It breaks the cycle in the previous resource graph and thus avoid deadlock.

Resource graph:

Please refer to the t2/Question3/question3-5-resource graph after my proposed change.jpg to see the corresponding resource graph.