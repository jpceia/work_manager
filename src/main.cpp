/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:14:16 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/02 04:41:07 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "WorkManager.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdio>

// https://codereview.stackexchange.com/questions/201640/a-thread-safe-task-queue-implementation-using-my-own-lock-guard-in-c

class MyTask : public wm::Task
{
public:
    MyTask(wm::Mutex& mutex) : _mutex(mutex) {}
    ~MyTask() {}

    void run()
    {
        sleep(1);
        wm::LockGuard lock(_mutex);
        printf("Task %d is running\n", this->getId());
    }

private:
    // Non-copyable
    MyTask(MyTask const &rhs) : _mutex(rhs._mutex) { (void)rhs; };
    MyTask &operator=(MyTask const &rhs) { (void)rhs; return *this; };

    // Private attributes
    wm::Mutex& _mutex;
};

int main()
{
    std::cout << "Hello World!" << std::endl;
    wm::WorkManager manager(5);
    manager.start();
    wm::Mutex mutex;
    MyTask *task[5];
    for (int i = 0; i < 5; i++)
        task[i] = new MyTask(mutex);
    task[0]->addDependency(task[1]);
    task[0]->addDependency(task[2]);
    task[1]->addDependency(task[3]);
    task[1]->addDependency(task[4]);
    task[2]->addDependency(task[3]);
    task[2]->addDependency(task[4]);
    for (int i = 0; i < 5; i++)
        manager.pushTask(task[i]);
    manager.wait();
    return 0;
}
