/*  Torcs Car Setup Editor. A visual car setup editor for torcs build using glui
    Copyright (C) 2007  Vicente Marti

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include <dirent.h>
#include <iostream> // para el cout
#include <vector>
using namespace std;


#include <sys/stat.h>

#ifdef WIN32
int isdir ( struct dirent *ent ){
    struct stat dr;
    if (!stat(ent->d_name,&dr) && S_ISDIR(dr.st_mode))
        return 1; /* dir is directory   */
    else
        return 0; /* dir is a file   */
}    
#else
int isdir ( struct dirent *ent ){
    struct stat dr;
    if (!lstat(ent->d_name,&dr) && S_ISDIR(dr.st_mode))
        return 1; /* dir is directory   */
    else
        return 0; /* dir is a file   */  
} 
#endif //WIN32

/* this function shows the dirs and files of dirPath
and saves these dirs in the vector directory */
int listDir (std::string dirPath, vector<string> &directori )
{
    cout << "Folders in: " << dirPath << endl;
    cout << "-------------------------" << endl;
    
    /* Present workind directory: the directory whre the program is placed */
    char pwd_str[500] = "";
    //cout << getcwd(pwd_str,500) << endl;
    getcwd(pwd_str,500);
    
    /* change the dir to the dirPath */
    chdir(dirPath.c_str());
    
    struct stat dr;
    DIR *dir = opendir("."); 
      
        if(dir){
            struct dirent *ent;
            int j = 0;
            while((ent = readdir(dir)) != NULL){
                if ( isdir( ent ) ){
                    if ( j > 1 ) /* we dont put to the list the ./ and ../ */
                        directori.push_back(ent->d_name);
                    j++;
                    cout << "Folder: " << ent->d_name << endl;
                }
                else {
                    cout << "File: " << ent->d_name << endl;
                }        
            }
            closedir(dir);
        }
        else{
            fprintf(stderr, "Error opening directory\n");
        }
    cout << endl;
    
    /* Go to the original present working directory */
    chdir(pwd_str);
}    


