import json
import math

tileMappings = {}

def load_json_data(filename):
    with open(filename, 'r') as file:
        return json.load(file)

def load_tileset_data(filename):
    with open(filename, 'r') as file:
        return json.load(file)

def process_level_data(data, tileset_data):
    collision_data = {}
    type_data = {}

    # Load tile properties
    for tile in tileset_data['tiles']:
        tile_id = tile['id']
        for prop in tile['properties']:
            if prop['name'] == 'collision':
                collision_data[tile_id] = prop['value']
            if prop['name'] == 'type':
                type_data[tile_id] = prop['value']

    # Process the level
    for layer in data['layers']:
        
        tiles = []
        for chunk in layer['chunks']:
            
            chunk_data = chunk["data"]
            for i in range(len(chunk_data)):
                
                if chunk_data[i] == 0: continue
                tile_id = chunk_data[i] - 1
                tile = {
                    "collision": collision_data.get(tile_id, False),
                    "tileset": 0,
                    "tilesetX": tile_id % tileset_data['columns'],
                    "tilesetY": math.floor(tile_id / tileset_data['columns']),
                    "type": type_data.get(tile_id, 1),
                    "x": ((i % chunk["width"]) + chunk["x"]) * 50,
                    "y": (math.floor(i / chunk["height"]) + chunk["y"]) * 50
                }
                tiles.append(tile)

        converted_data = {
            "tiles": tiles
        }
        with open("../assets/level.json", "w") as file:
            json.dump(converted_data, file, indent=4)
        print("Level data written to ../assets/level.json")

def main():
    level_filename = 'TiledData/demoLevel.json'
    tileset_filename = 'TiledData/tileset.json'
    level_data = load_json_data(level_filename)
    tileset_data = load_tileset_data(tileset_filename)
    process_level_data(level_data, tileset_data)

if __name__ == "__main__":
    main()
