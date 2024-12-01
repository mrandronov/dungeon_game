import matplotlib.pyplot as plt
from perlin_noise import PerlinNoise

width = 50
height = 50

def GetVal( x, y ):
    noise1 = PerlinNoise(octaves=3)
    noise2 = PerlinNoise(octaves=6)
    noise3 = PerlinNoise(octaves=12)
    noise4 = PerlinNoise(octaves=24)

    noise_val = noise1( [x, y] )
    noise_val += 0.5 * noise2( [x, y] )
    noise_val += 0.25 * noise3( [x, y] )
    noise_val += 0.125 * noise4( [x, y] )

    return noise_val

def GenerateMap( filename ):
    file = open( filename, "w" )
    for i in range( width ):
        for j in range( height ):
            val = GetVal( i / width, j / height ) 
            if val > 0:
                val = 3
            else:
                val = 1
            file.write( str(val) + ' ' )
        file.write('\n')
    file.close()


GenerateMap("tilemap3.map")

