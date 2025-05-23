#pragma once

#include <string>

/*  Action class that describes the action name such as "jump", "right", etc.
    together with its types "start" and "end"
*/

class Action
{
private:
    std::string m_name{"NONE"};
    std::string m_type{"NONE"};

public:
    // constructors
    Action();
    Action(const std::string& name, const std::string& type);

    const std::string& name() const;
    const std::string& type() const;
    std::string toString() const;
};
