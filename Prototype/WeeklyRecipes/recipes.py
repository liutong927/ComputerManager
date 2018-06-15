# a script to generate daily dishes from reading json file.
import json
import random

# generate dinner from lists of unknown number.
def generate_dinner_args(*args):
    dinner = []

    for recipe_list in args:
        selected_recipe = random.choice(recipe_list)
        dinner.append(selected_recipe)
        recipe_list.remove(selected_recipe)
        # print(recipe_list)

    return dinner

# but what if we want specify numbers of vegetables or meats. so crete kwargs version function.
def generate_dinner_kwargs(num_vegetables=1, num_meats=1, **kwargs):
    dinner = []
    #print(kwargs)

    for key in kwargs.keys():
        recipes_list = [] # selected list in one loop.
        # use random.sample here instead of random.choices since later can choose same element.
        if key == 'vegetables':
            recipes_list = random.sample(kwargs[key], k=num_vegetables)
        elif key == 'meats':
            recipes_list = random.sample(kwargs[key], k=num_meats)
        else:
            recipes_list = random.sample(kwargs[key], k=1)

        for selected_recipe in recipes_list:
            dinner.append(selected_recipe)
            if selected_recipe in kwargs[key]:
                kwargs[key].remove(selected_recipe)

    return dinner

# generate one day dinner, no duplicates!
def generate_daily_dinner(vegetables, meats, soups):
    # daily dinner should have no duplicates. pass list reference.
    #lunch = generate_dinner_args(vegetables, meats, soups)
    #supper = generate_dinner_args(vegetables, meats, soups)

    lunch = generate_dinner_kwargs(1, 2, vegetables=vegetables, meats=meats, soups=soups)
    supper = generate_dinner_kwargs(1, 2, vegetables=vegetables, meats=meats, soups=soups)

    daily_dinner = {'lunch': lunch, 'supper': supper}
    return daily_dinner


if __name__ == '__main__':
    with open('recipes.json') as file:
        recipes = json.load(file)

    vegetables = recipes['素菜']
    meats = recipes['荤菜']
    soups = recipes['汤']
    print('素菜=', vegetables, '荤菜=', meats, '汤=', soups, sep='\n')

    print('Today\'s dinner=', generate_daily_dinner(vegetables, meats, soups))
