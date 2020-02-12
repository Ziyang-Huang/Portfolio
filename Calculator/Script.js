var expression = document.getElementById("expressionDisplay");
var info = document.getElementById("info");
var pointFlag = false;
var operatorFlag = false;
var number = "";

function numBtnOnClick(btn){
    if(number == '0'){
        if(btn.id == '0'){
            return ;
        }
        else{
            expression.value = expression.value.substr(0, expression.value.length - 1) + btn.id;
            number = btn.id;
            return ;
        }
    }
    expression.value += btn.id;
    number += btn.id;
    operatorFlag = false;
    info.textContent = "";
}

function onReset(){
    expression.value = "";
    info.textContent = "";
    pointFlag = false;
    operatorFlag = false;
    number = "";
}

function PointBtnOnClick(){
    if(!pointFlag){
        expression.value += '.';
        number += '.';
        pointFlag = true;
        info.textContent = "";
    }
    else{
        info.textContent = "A point has been in this number!";
    }
}

function opBtnOnClick(btn){
    if(!expression.value){
        if(btn.id == "*" || btn.id == "/"){
            info.textContent = 'An expression cannont start with an operater except "+" and "-".' 
            return ;
        }
    }
    if(expression.value == "+" || expression.value == "-"){
        if(btn.id == "*" || btn.id == "/"){
            info.textContent = 'An expression cannont start with an operater except "+" and "-".' 
            return ;
        }
        else{
            expression.value = expression.value.substr(0, expression.value.length - 1) + btn.id;
            info.textContent = "";
            return ;
        }
    }
    if(expression.value.substr(expression.value.length - 1, 1) == '.'){
        info.textContent = '"." is not a number!\nOperator is invalid!';
        return;
    }
    if(operatorFlag){
        expression.value = expression.value.substr(0, expression.value.length - 1) + btn.id;
        info.textContent = "";
    }
    else{
        expression.value += btn.id;
        number = "";
        operatorFlag = true;
        pointFlag = false;
        info.textContent = "";
    }
}

function delBtnOnClick(){
    if(expression.value){
        expression.value = expression.value.substr(0, expression.value.length - 1);
        info.textContent = "";
    }
}

function equalBtnOnClick(){
    if(!expression.value){
        return ;
    }
    try{
        expression.value = eval(expression.value);
        info.textContent = "";
    }
    catch(err){
        if(err.name == "SyntaxError"){
            info.textContent = "SyntaxError: Wrong expression!";
        }
    }
}
