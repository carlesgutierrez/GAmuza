-- image Example

test = of.image()

function setup()
    test:loadImage("img/ravenShammy.png")
end

function draw()
	ga.background(0.0,1.0)
    of.setColor(255,255,255)
    test:draw(0,0,OUTPUT_W,OUTPUT_H)
end