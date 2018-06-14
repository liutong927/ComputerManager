import json
import random


# generate dinner from lists of unknown number.
def generate_dinner(*args):
    dinner = []

    for recipe_list in args:
        selected_recipe = random.choice(recipe_list)
        dinner.append(selected_recipe)
        recipe_list.remove(selected_recipe)
        # print(recipe_list)

    return dinner

# but what if we want specify numbers of vegetables or meats.
def generate_dinner_kargs(numVegetables=1, numMeats=1, **kwargs):
    dinner = []


    for key in kwargs.keys():
        recipes_list = [] # selected list in one loop.
        if key == 'vegetables':
            #todo: seems still not choose 2 different dish.
            recipes_list = random.choices(kwargs[key], k=numVegetables)
        elif key == 'meats':
            recipes_list = random.choices(kwargs[key], k=numMeats)
        else:
            recipes_list = random.choices(kwargs[key], k=1)

        for selected_recipe in recipes_list:
            dinner.append(selected_recipe)
            #todo:still have duplicates for supper.also has ValueError for x not in the list.
            kwargs[key].remove(selected_recipe)

    return dinner

# generate one day dinner, no duplicates!
def generate_daily_dinner(vegetables, meats, soups):
    # daily dinner should have no duplicates. pass list reference.
    #lunch = generate_dinner(vegetables, meats, soups)
    #supper = generate_dinner(vegetables, meats, soups)

    lunch = generate_dinner_kargs(1, 2, vegetables=vegetables, meats=meats, soups=soups)
    supper = generate_dinner_kargs(1, 2, vegetables=vegetables, meats=meats, soups=soups)

    daily_dinner = {'lunch': lunch, 'supper': supper}
    print(daily_dinner)

    return daily_dinner


if __name__ == '__main__':
    with open('recipes.json') as file:
        recipes = json.load(file)

    vegetables = recipes['素菜']
    meats = recipes['荤菜']
    soups = recipes['汤']
    print(vegetables, meats, soups, sep='\n')

    generate_daily_dinner(vegetables, meats, soups)
