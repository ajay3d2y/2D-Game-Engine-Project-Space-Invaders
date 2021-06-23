function MovPlatform(a){
    if((a.x() == a.Dx()) && (a.y() == a.Dy())){
        print("New Destination");
        a.newDes();
    }
    else{
        s = a.speed();
        x = a.x() + (a.dx()/s);
        y = a.y() + (a.dy()/s);
        a.setPos(x,y);
        }
    return 'Success!';
}