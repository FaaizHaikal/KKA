from constraint import Problem

# Create a CSP problem instance
problem = Problem()

# Define the variables and domains
classes = ["Class1", "Class2", "Class3", "Class4", "Class5"]
professors = ["A", "B", "C"]

# Add variables to the problem with a custom domain using a lambda function
for c in classes:
    problem.addVariable(c, lambda professor: professor in professors)

# Define the constraints
def professor_availability_constraint(Class, Professor):
    professor_availability = {
        "Class1": ["C"],
        "Class2": ["B", "C"],
        "Class3": ["A", "B", "C"],
        "Class4": ["A", "B", "C"],
        "Class5": ["B", "C"],
    }
    return Professor in professor_availability[Class]

def class_schedule_constraint(Class, StartTime):
    class_schedule = {
        "Class1": (8, 9),
        "Class2": (8.5, 9.5),
        "Class3": (9, 10),
        "Class4": (9, 10),
        "Class5": (9.5, 10.5),
    }
    return StartTime >= class_schedule[Class][0] and StartTime <= class_schedule[Class][1]

# Add constraints to the problem
for c in classes:
    problem.addConstraint(professor_availability_constraint, (c,))
    problem.addConstraint(class_schedule_constraint, (c,))

# Find a solution
solutions = problem.getSolutions()

# Print the solutions
for solution in solutions:
    print("Solution:")
    for c in classes:
        print(f"{c}: Professor {solution[c]}")
    print()
