# Kelompok    : Bocil AI
# Kelas       : KKA E
# Dosen       : Bu Chastine
# Programmer  : Faa'iz Haikal Hilmi (5025221219)
#             :
#             :
#
# [PROBLEM DESCRIPTION]

class TreeNode:
    def __init__(self, floor, weight, heuristic):
        self.floor = floor
        self.weight = weight
        self.heuristic = heuristic
        self.children = []

    def add_child(self, obj):
        self.children.append(obj)
    
    # def __str__(self, level=0):
    #     ret = " " * level * 4 + str(self.floor) + "(" + str(self.weight) + " " + str(self.heuristic) + ")" + "\n"
    #     for child in self.children:
    #         ret += child.__str__(level + 1)
    #     return ret

# PERMUTATIONS OF FLOORS
def create_elevator_orders(arr):
    if len(arr) == 0:
        return [[]]
    elif len(arr) == 1:
        return [arr]

    result = []
    for i in range(len(arr)):
        first_elem = arr[i]
        rest = arr[:i] + arr[i+1:]

        # CHECK IF DESTINATION IS PICKED BEFORE SOURCE
        if first_elem in destination_to_source:
            source = destination_to_source[first_elem]
            if source in rest:
                continue
        
        for p in create_elevator_orders(rest):
            temp = [first_elem] + p
            result.append(temp)
    return result

def build_tree_from_paths(paths):
    # CREATE ROOT NODE
    root = TreeNode(initial_floor, 0 , total_people)

    for path in paths:
        current_node = root
        for value in path:
            child_node = None
            for child in current_node.children:
                if child.floor == value:
                    child_node = child
                    break
            if not child_node:
                w = abs(value - current_node.floor)         # WEIGHT

                # HEURISTIC
                h = current_node.heuristic
                if value in destination_to_source:
                    s = destination_to_source[value]
                    h -= source_with_people[s]
                
                child_node = TreeNode(value, w, h)
                current_node.add_child(child_node)
            current_node = child_node
    
    return root

if __name__ == "__main__":
    initial_floor = int(input("Masukkan lantai awal: "))        # ELEVATOR INITIAL FLOOR
    request = int(input("Masukkan banyaknya permintaan: "))     # CALLS / REQUESTS FOR ELEVATOR
    floors = []
    source_with_people = {}
    destination_to_source = {}
    total_people = 0

    # INPUT
    for i in range(request):
        source_floor, target_floor, people = input("Masukkan lantai asal, lantai tujuan, dan jumlah orang: ").split()
        source_floor = int(source_floor)
        target_floor = int(target_floor)
        people = int(people)

        floors.extend([source_floor, target_floor])         # LIST OF FLOORS
            
        source_with_people[source_floor] = people           # SOURCE FLOOR WITH PEOPLE
        destination_to_source[target_floor] = source_floor  # DESTINATION FLOOR WITH SOURCE FLOOR
        total_people += people                              # TOTAL PEOPLE

    # CREATE VALID ELEVATOR ORDERS
    elevator_orders = create_elevator_orders(floors)
    
    # CLEAN UP
    del floors

    # BUILD TREE
    root_node = build_tree_from_paths(elevator_orders)
