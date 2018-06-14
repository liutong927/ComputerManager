import random

# generate dinner from lists of unknown number.
def generate_dinner(*args):
    dinner = []

    for recipe_list in args:
        dinner.append(random.choice(recipe_list))

    print(dinner)


if __name__ == '__main__':
    breakfasts = ['a', 'b', 'c']
    vegatables = ['h', 'i', 'j', 'k']
    meats = ['o', 'p', 'q', 'r']
    soups = ['x', 'y', 'z']
    generate_dinner(breakfasts, vegatables, meats, soups)
    generate_dinner(vegatables, meats, soups)
