using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;

namespace TemperatureStation.App.Behaviors
{
    class ListViewNoSelectionBehavior : Behavior<ListView>
    {
        protected override void OnAttachedTo(ListView bindable)
        {
            bindable.ItemSelected += OnItemSelected;
            base.OnAttachedTo(bindable);
        }

        protected override void OnDetachingFrom(ListView bindable)
        {
            bindable.ItemSelected -= OnItemSelected;
            base.OnDetachingFrom(bindable);
        }

        private void OnItemSelected(object sender, SelectedItemChangedEventArgs selectedItemChangedEventArgs)
        {
            if (sender is ListView listView)
            {
                listView.SelectedItem = null;
            }
        }
    }
}
