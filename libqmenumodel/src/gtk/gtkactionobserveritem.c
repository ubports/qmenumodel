
#include "gtkactionobserveritem.h"

typedef GObjectClass GtkActionObserverItemClass;

struct _GtkActionObserverItem
{
  GObject parent_instance;
  GtkActionObservable *observable;
  gchar* action_name;

  GtkActionAddedFunc action_added;
  GtkActionEnabledChangedFunc action_enabled_changed;
  GtkActionStateChangedFunc action_state_changed;
  GtkActionRemovedFunc action_removed;
};

static void gtk_action_observer_item_init_observer_iface (GtkActionObserverInterface *iface);
G_DEFINE_TYPE_WITH_CODE (GtkActionObserverItem, gtk_action_observer_item, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (GTK_TYPE_ACTION_OBSERVER, gtk_action_observer_item_init_observer_iface))

static void
gtk_action_observer_item_finalize (GObject *object)
{
  GtkActionObserverItem *self = GTK_ACTION_OBSERVER_ITEM (object);

  if (self->observable)
    g_object_unref (self->observable);

  if (self->action_name)
    g_free(self->action_name);

  G_OBJECT_CLASS (gtk_action_observer_item_parent_class)->finalize (object);
}

static void
gtk_action_observer_item_init (GtkActionObserverItem * self)
{
}

static void
gtk_action_observer_item_class_init (GtkActionObserverItemClass *class)
{
  class->finalize = gtk_action_observer_item_finalize;
}

static void
gtk_action_observer_item_action_added (GtkActionObserver   *observer,
                                    GtkActionObservable *observable,
                                    const gchar         *action_name,
                                    const GVariantType  *parameter_type,
                                    gboolean             enabled,
                                    GVariant            *state)
{
  g_return_if_fail (GTK_IS_ACTION_OBSERVER_ITEM (observer));

  GtkActionObserverItem* self;
  self = GTK_ACTION_OBSERVER_ITEM (observer);
  self->action_added(self, action_name, enabled, state);
}

static void
gtk_action_observer_item_action_enabled_changed (GtkActionObserver   *observer,
                                              GtkActionObservable *observable,
                                              const gchar         *action_name,
                                              gboolean             enabled)
{
  g_return_if_fail (GTK_IS_ACTION_OBSERVER_ITEM (observer));

  GtkActionObserverItem* self;
  self = GTK_ACTION_OBSERVER_ITEM (observer);
  self->action_enabled_changed(self, action_name, enabled);
}

static void
gtk_action_observer_item_action_state_changed (GtkActionObserver   *observer,
                                            GtkActionObservable *observable,
                                            const gchar         *action_name,
                                            GVariant            *state)
{
  g_return_if_fail (GTK_IS_ACTION_OBSERVER_ITEM (observer));

  GtkActionObserverItem* self;
  self = GTK_ACTION_OBSERVER_ITEM (observer);
  self->action_state_changed(self, action_name, state);
}

static void
gtk_action_observer_item_action_removed (GtkActionObserver   *observer,
                                      GtkActionObservable *observable,
                                      const gchar         *action_name)
{
  g_return_if_fail (GTK_IS_ACTION_OBSERVER_ITEM (observer));

  GtkActionObserverItem* self;
  self = GTK_ACTION_OBSERVER_ITEM (observer);
  self->action_removed(self, action_name);
}

static void
gtk_action_observer_item_init_observer_iface (GtkActionObserverInterface *iface)
{
  iface->action_added = gtk_action_observer_item_action_added;
  iface->action_enabled_changed = gtk_action_observer_item_action_enabled_changed;
  iface->action_state_changed = gtk_action_observer_item_action_state_changed;
  iface->action_removed = gtk_action_observer_item_action_removed;
}

GtkActionObserverItem*
gtk_action_observer_item_new (GtkActionObservable *observable,
                              GtkActionAddedFunc action_added,
                              GtkActionEnabledChangedFunc action_enabled_changed,
                              GtkActionStateChangedFunc action_state_changed,
                              GtkActionRemovedFunc action_removed)
{
  GtkActionObserverItem* self;
  self = g_object_new (GTK_TYPE_ACTION_OBSERVER_ITEM, NULL);
  self->observable =  g_object_ref (observable);
  self->action_name = NULL;

  self->action_added = action_added;
  self->action_enabled_changed = action_enabled_changed;
  self->action_state_changed = action_state_changed;
  self->action_removed = action_removed;

  return self;
}

void
gtk_action_observer_item_register_action (GtkActionObserverItem *self,
                                         const gchar* action_name)
{
  gtk_action_observer_item_unregister_action(self);

  if (action_name && g_strcmp0(action_name, "") != 0) {
    self->action_name = g_strdup (action_name);

    if (g_strcmp0(self->action_name, "") != 0) {
      gtk_action_observable_register_observer (self->observable, self->action_name, GTK_ACTION_OBSERVER (self));
    }
  }
}

void
gtk_action_observer_item_unregister_action (GtkActionObserverItem *self)
{
  if (self->action_name) {
    gtk_action_observable_unregister_observer(self->observable, self->action_name, GTK_ACTION_OBSERVER (self));
    g_free(self->action_name);
    self->action_name = NULL;
  }
}
