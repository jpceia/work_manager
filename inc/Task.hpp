/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:20:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/09 01:30:36 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASK_HPP
# define TASK_HPP

# include "Node.hpp"
# include "safe/Set.hpp"
# include "safe/Queue.hpp"
# include "BaseTask.hpp"

namespace wm
{

class Task : public BaseTask, public Node<Task>
{
public:
    Task();
    virtual ~Task();

    bool isLocked() const;
    static bool isLocked(Task *task);

    void lock();
    static void lock(Task *task);

    void moveDeps(SafeSet<Task>& taskSet, SafeQueue<Task>& taskQueue);

private:
    // Helper methods
    bool _moveIfReady(SafeSet<Task>& taskSet, SafeQueue<Task>& taskQueue);

    // Private attributes
    bool _locked;
};

} // namespace wm

#endif
