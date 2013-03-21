using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;

namespace UIHeavyClient
{
    public static class RazerUtilities
    {
        public static IEnumerable<T> FindTypedChildren<T>(DependencyObject depObj, bool recursive) where T : DependencyObject
        {
            if (depObj != null)
            {
                foreach (var child in LogicalTreeHelper.GetChildren(depObj))
                {
                    //DependencyObject child = LogicalTreeHelper.GetChild(depObj, i);
                    if (child != null && child is T)
                    {
                        yield return (T)child;
                    }

                    if (recursive)
                    {
                        foreach (T childOfChild in FindTypedChildren<T>(child as DependencyObject, true))
                        {
                            yield return childOfChild;
                        }
                    }
                }
            }
        }


    }
}
