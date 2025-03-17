# Questions and Answers

**Question**: Is the order of the messages consistent?  
**Answer**: The order of the messages printed by the child processes is **not consistent** across runs. The order depends on the **scheduling of the processes by the operating system**.

---

**Question**: Can the order of these messages be predicted?  
**Answer**: The order of the messages cannot be predicted because it depends on the operating system's process scheduling.

---

**Question**: What does it depend on?  
**Answer**: The order of the messages depends on the **scheduling of the processes by the operating system**, which is influenced by factors like system load, CPU availability, and process priorities.

---

**Question**: Does it matter whether you seed the random number generator using `srand(getpid())` before or after the call to `fork()`?  
**Answer**: Yes, it matters. If `srand(getpid())` is called **before** `fork()`, all child processes will inherit the same seed and generate the same sequence of random numbers. If it is called **after** `fork()`, each child process will have a unique seed and generate different sequences of random numbers.

---

**Question**: Explain your answer.  
**Answer**: Seeding with `srand(getpid())` ensures that each child process has a unique seed because each process has a unique PID. If seeding is done **before** `fork()`, all child processes will use the same seed, resulting in identical random sequences. If seeding is done **after** `fork()`, each child process will use its own PID as the seed, ensuring unique random sequences.