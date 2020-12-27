# 42_philosophers
Handling shared memory usage by multiple threads and processes with the classic Dining Philosophers Problem.

./philoNB number_of_philosophers time_to_die time_to_eat time_to_sleep number_of_times_each_philosopher_must_eat(option)

(times are given in milliseconds)

**COMMON RULES:**
  - Philosophers eat, then sleep, then think.
  - There are as many forks as philosophers.
  - A philosopher needs 2 forks in order to eat.
  - Philosopher don't know when another is about to die
  -  Each time a philosopher has finished eating, he will drop his forks and start sleeping.
  - The simulation stops when a philosopher dies
  - If a philosopher doesn’t start eating ’time_to_die’ milliseconds after starting his last meal or the beginning of the simulation, it dies
  - There must be less than 10ms between the death of a philosopher and the corresponding printed message
  
**PHILO_ONE ADDITIONAL RULES**
  - One fork between each philosopher, therefore there will be a fork at the right and at the left of each philosopher.
  - Each fork should be protected by a mutex
  - Each philosopher should be a thread.

**PHILO_TWO ADDITIONAL RULES**
  - All forks are in the middle of the table
  - They have no states in memory but the number of available forks is represented by a semaphore.
  - Each philosopher should be a thread.

**PHILO_THREE ADDITIONAL RULES**
  - All forks are in the middle of the table
  - They have no states in memory but the number of available forks is represented by a semaphore.
  - Each philosopher should be a process and the main process shouldn't be a philosopher.



Written in november 2020.
