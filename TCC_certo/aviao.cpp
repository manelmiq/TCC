/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   aviao.cpp
 * Author: emmanuell
 * 
 * Created on 14 de Março de 2016, 13:35
 */


#include<string>

#include"aviao.h"


aviao::aviao(std::string prefixo, std::string modelo){
    this->prefixo = prefixo;
    this->modelo = modelo;
}



aviao::aviao(const aviao& orig) {
}

aviao::~aviao() {
}

