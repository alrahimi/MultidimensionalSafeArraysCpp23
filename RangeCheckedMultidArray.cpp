#include "RangeCheckedMultidArray.h"

int main() {
    // Using the alias for a 3D grid
    GridTools::Array3D<float, 5, 10, 10> sensorData;
    
    sensorData.fill(0.0f);
    
    
    
    // Using the alias for a 4D grid
    GridTools::Array4D<float, 11, 12, 13,14> Tensor4d;
    
    Tensor4d.fill(1234.0f);

    try {
        sensorData[4][9][9] = 98.6f;
        std::cout << "Data saved successfully: " << sensorData[4][9][9] << std::endl;

        
              
        Tensor4d[1][2][3][4] = 4321.0f;
        std::cout << "Tensor4d[1][2][3][4]= " << Tensor4d[1][2][3][4] << std::endl;
        
        sensorData[4][9][9] = 499.0f;
        std::cout << "Data saved successfully: " << sensorData[4][9][9] << std::endl;
        
                
        // This will trigger the error and report it's at 'Dimension Level 2' (Rows)
        sensorData[0][15][0] = 1.0f; 
  

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}