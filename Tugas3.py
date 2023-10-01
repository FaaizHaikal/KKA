from itertools import permutations

class TreeNode:
    def __init__(self, floor, weight, heuristic):
        self.floor = floor
        self.weight = weight
        self.heuristic = heuristic
        self.children = []

    def add_child(self, obj):
        self.children.append(obj)
    
    def __str__(self, level=0):
        ret = " " * level * 4 + str(self.floor) + "( " + str(self.weight) + " " + str(self.heuristic) + " )" + "\n"
        for child in self.children:
            ret += child.__str__(level + 1)
        return ret

def build_tree_from_paths(paths):
    root = TreeNode(initialFloor, 0 , totalPeople)
    for path in paths:
        current_node = root
        i = 1
        while i < len(path):
            value = path[i]
            child_node = None
            for child in current_node.children:
                if child.floor == value:
                    child_node = child
                    break
            if not child_node:
                w = abs(value - current_node.floor)
                h = current_node.heuristic
                if value in originalPairs:
                    h += originalPairs[value][0]
                child_node = TreeNode(value, w, h)
                current_node.add_child(child_node)
            current_node = child_node
            i += 1
    return root

# TODO: Modify the permutation itself to reduce time complexity
def create_elevator_orders():
    orders = list(permutations(floors))
    validOrders = []
    for order in orders:
        if order[0] not in originalPairs:
            continue
        i = 1
        valid = True
        while i < len(order):
            if order[i] not in originalPairs:
                currentFloor = order[i]
                source = destinationToSource[currentFloor]
                sourceIndex = order.index(source)
                if sourceIndex > i:
                    valid = False
                    break
            i += 1
        if valid:
            order = [initialFloor] + list(order)
            validOrders.append(order)
    
    return validOrders

initialFloor = int(input("Masukkan lantai awal: "))
requestNumber = int(input("Masukkan banyaknya permintaan: "))
floors = []
originalPairs = {}
destinationToSource = {}
totalPeople = 0
for i in range(requestNumber):
    sourceFloor, targetFloor, people = input("Masukkan lantai asal, lantai tujuan, dan jumlah orang: ").split()
    sourceFloor = int(sourceFloor)
    targetFloor = int(targetFloor)
    people = int(people)

    floors.extend([sourceFloor, targetFloor])

    originalPairs[sourceFloor] = [people]
    destinationToSource[targetFloor] = sourceFloor
    totalPeople += people

elevatorOrders = create_elevator_orders()
root_node = build_tree_from_paths(elevatorOrders)

print(root_node)
