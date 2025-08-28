#ifndef COLUMNCONSTRAINT_H
#define COLUMNCONSTRAINT_H

#include <QObject>
#include <QMap>

enum class ConstraintType {                         // Column constraint types
    EnumSet,
    Range,
    Regex,
    Custom
};

struct ColumnConstraint {                           // Column constraint structure
    ConstraintType type;                            // Type of constraint
    virtual ~ColumnConstraint() = default;          // Constraint properties object
};

/**
 * @brief The enumeration constraint class
 */
struct EnumConstraint : public ColumnConstraint {
    QMap<int, QString> valueLabels;                 // e.g. { 0: "Open", 1: "Marked", 2: "Reconciled" }

    /**
     * @brief Enumeration constraint class constructor
     */
    EnumConstraint() {
        type = ConstraintType::EnumSet;
    }

    /**
     * @brief Retrieve label for enumerated value
     *
     * @param value Value of enumeration
     * @returns QString label associated with value or "Unknown"
     */
    QString labelFor(int value) const {
        return valueLabels.value(value, QString("Unknown"));
    }

    /**
     * @brief Retrieve list of enumeration values
     *
     * @returns Integer list of values
     */
    QList<int> allowedValues() const {
        return valueLabels.keys();
    }
};

#endif // COLUMNCONSTRAINT_H
