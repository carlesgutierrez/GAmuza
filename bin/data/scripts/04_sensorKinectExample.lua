

activeBlobs = 0

----------------------------------------------------
function setup()
    
end

----------------------------------------------------
function update()
    activeBlobs = ga.oniBlobs()
end

----------------------------------------------------
function draw()
    
    ga.background(0.0,1.0)
    of.setColor(255,255,0)
    of.fill()
    for i=0,activeBlobs do
        of.ellipse(ga.oniBlobX(i)*OUTPUT_W,ga.oniBlobY(i)*OUTPUT_H,ga.oniBlobW(i)*OUTPUT_W,ga.oniBlobH(i)*OUTPUT_H)
    end
end















