#version 330 core

uniform int u_TableWidth;
uniform int u_TableHeight;
uniform int u_CellSize;
uniform float u_Table[676];

const int screenSize = 640;

int hash_index(vec2 position) {
    // int(position/cellSize) makes size of pixel adjustable becouse for eg. pixelSize = 2 position = 1 : int(1/2) = 0 but position = 2 : int(2/2) = 2
    // it just adds delay to next position in table
    // taking modulo of pixel position and width/height gives us value from 0 to width/height -1 so perfect for indexing in table and there is no index error
    // multiplying the y pixel position by table width takes index to the next row
    // int(mod()) becouse modulo returns float
    return int(mod(int(position[0]/u_CellSize), u_TableWidth) + mod(int(position[1]/u_CellSize), u_TableHeight)*u_TableWidth);
}

void main()
{
    int index = hash_index(gl_FragCoord.xy);
    float cell_state = u_Table[index];

    vec3 color = vec3(cell_state);

    gl_FragColor = vec4(color, 1.0);
}
