function movEnemy(a){
    var sx = 0.5;
    var sy = 0.1;
    var y = a.y();
    y = y + sy;
    if(a.direction()>0){
        if(a.offSet()>=30){
            a.chngDirection();
        }
        else{
            var x = a.x();
            var o = a.offSet()
            x = x + sx;
            o = o + sx;
            a.setPos(x,y,o);
        }
    }
    else{
        if(a.offSet()<=-50){
            a.chngDirection();
        }
        else{
            var x = a.x();
            var o = a.offSet();
            x = x - sx;
            o = o - sx;
            a.setPos(x,y,o);
        }
    }
    return "Successful";
}

