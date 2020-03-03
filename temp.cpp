 int numberLines = 5;
    string **tablaFinal = new string*[numberLines];

    for(int i = 0; i<numberLines; i++){
        tablaFinal[i] = new string[4];

    }
    tablaFinal[0][0]= "hola";
    
        for(int i = 0; i<5; i++){
            for(int j=0; j<4; j++){
                cout<<i<<j<<tablaFinal[i][j];
            }
            cout<<endl;
        }



int getNumberOfLines(string fileName){
    ifstream* inFile;
    string lineRead; 
    int i = 0;
    inFile = new ifstream(fileName);
        if(!*inFile){
            cerr<< "no se puede abrir";
        }else{
           
             while(getline(*inFile, lineRead))
             {
              i++; 
            }
            inFile->close();
      
}
    return i;
}