﻿using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SvnClient.Backend
{
    public interface IMessenger
    {
        void Register<TMessage>(Action<TMessage> action);
        void Send<TMessage>(TMessage message);
        void UnRegister<TMessage>(Action<TMessage> action);
    }

    public class Messenger : IMessenger
    {
        private readonly ConcurrentDictionary<Type, ConcurrentBag<Delegate>> registeredActions = new ConcurrentDictionary<Type, ConcurrentBag<Delegate>>();
        private readonly Object bagLock = new Object();

        public void Register<TMessage>(Action<TMessage> action)
        {
            var key = typeof(TMessage);

            lock (bagLock)
            {
                ConcurrentBag<Delegate> actions;
                if (!registeredActions.TryGetValue(typeof(TMessage), out actions))
                {
                    actions = new ConcurrentBag<Delegate>();
                    registeredActions[key] = actions;
                }

                actions.Add(action);
            }

        }

        public void Send<TMessage>(TMessage message)
        {
            ConcurrentBag<Delegate> actions;
            if (registeredActions.TryGetValue(typeof(TMessage), out actions))
            {
                foreach (var action in actions.Select(a => a as Action<TMessage>).Where(a => a != null))
                {
                    action(message);
                }
            }
        }

        public void UnRegister<TMessage>(Action<TMessage> action)
        {
            var key = typeof(TMessage);

            ConcurrentBag<Delegate> actions;
            if (registeredActions.TryGetValue(typeof(TMessage), out actions))
            {
                lock (bagLock)
                {
                    var actionsList = actions.ToList();
                    actionsList.Remove(action);
                    registeredActions[key] = new ConcurrentBag<Delegate>(actionsList);
                }
            }
        }
    }
}
