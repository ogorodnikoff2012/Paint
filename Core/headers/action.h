#ifndef ACTION_H
#define ACTION_H

#include <QVariant>

class Action
{
public:
    enum ActionType {LAYER_CHANGED, LAYER_DELETED, LAYER_CREATED, LAYER_RENAMED};
    Action(const QVariant &data, ActionType type);
    ~Action();
    QVariant data;
    ActionType type;
};

#endif // ACTION_H
