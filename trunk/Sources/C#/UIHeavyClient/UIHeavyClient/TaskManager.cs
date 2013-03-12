using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Threading;
using System.Threading;

namespace UIHeavyClient
{
    public class TaskManager
    {
        private readonly TaskScheduler scheduler;
        private readonly TaskFactory factory;
        //private readonly CountdownEvent countdownEvent;

        // In this example, we initialize the countdown event with the total number
        // of child threads so that we know when all threads are finished scheduling
        // work.
        public TaskManager()
        {
            this.scheduler = TaskScheduler.FromCurrentSynchronizationContext();
            this.factory = new TaskFactory(this.scheduler);
        }

        // This method should be called by a child thread when it wants to invoke
        // an operation back on the main dispatcher thread.  This will block until
        // the method is done executing.
        public void ExecuteTask(Action action)
        {
            if(action != null)
            {
                try
                {
                    Task task = this.factory.StartNew(action);
                    // pour l'instant on n'attend pas que la task ce termine, car 
                    // le wait ne prend pas en compte le fait que la task peut etre deja
                    // terminé...
                    //task.Wait();
                }
                catch (System.ObjectDisposedException ex)
                {

                    System.Console.Out.WriteLine("ObjectDisposedException" + ex.Message);
                }
                catch (System.AggregateException ex)
                {
                    System.Console.Out.WriteLine("AggregateException" + ex.Message);
                }
                catch (System.InvalidOperationException ex)
                {
                    System.Console.Out.WriteLine("InvalidOperationException" + ex.Message);
                }
            }
        }
    }

}
