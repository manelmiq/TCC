/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   aviao.h
 * Author: emmanuell
 *
 * Created on 14 de Março de 2016, 13:35
 */


#ifndef AVIAO_H
#define AVIAO_H

class aviao {
public:
    std::string prefixo, modelo;
    
    aviao(std::string prefixo, std::string modelo);
  
    
    std::string GetModelo() const {
        return modelo;
    }

    void SetModelo(std::string modelo) {
        this->modelo = modelo;
    }

    std::string GetPrefixo() const {
        return prefixo;
    }

    void SetPrefixo(std::string prefixo) {
        this->prefixo = prefixo;
    }
    std::string toString(){
        return "Aviao modelo : "+modelo+ "prefixo "+ prefixo + "\n";
    }
    aviao();
    aviao(const aviao& orig);
    virtual ~aviao();
private:

};









#endif /* AVIAO_H */

