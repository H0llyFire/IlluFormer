<?xml version="1.0" encoding="UTF-8"?>

<tileset version="1.8" tiledversion="1.8.2" name="IlluFormerSprites" tilewidth="256" tileheight="256" tilecount="10" columns="0">
    <grid orientation="orthogonal" width="1" height="1"/>
    <tile id="0" type="Player">
        <properties>
            <property name="TextureName" value="Player"/>
        </properties>
        <image width="256" height="256" source="../textures/CharWalkFrame7.png"/>
    </tile>
    <tile id="1" type="Coin">
        <properties>
            <property name="TextureName" value="Coin"/>
        </properties>
        <image width="256" height="256" source="../textures/Coin.png"/>
    </tile>
    <tile id="2" type="Enemy">
        <properties>
            <property name="TextureName" value="Enemy"/>
        </properties>
        <image width="256" height="256" source="../textures/Enemy.png"/>
    </tile>
    <tile id="3" type="Flag">
        <properties>
            <property name="TextureName" value="Flag"/>
        </properties>
        <image width="256" height="256" source="../textures/Flag.png"/>
    </tile>
    <tile id="4" type="Ground">
        <properties>
            <property name="TextureName" value="Ground"/>
        </properties>
        <image width="256" height="256" source="../textures/Ground.png"/>
    </tile>
    <tile id="6" type="Wall">
        <properties>
            <property name="TextureName" value="Wall"/>
        </properties>
        <image width="256" height="256" source="../textures/Wall.png"/>
    </tile>
    <tile id="7" type="SpikeDown">
        <image width="256" height="256" source="../textures/SpikeDown.png"/>
    </tile>
    <tile id="8" type="SpikeLeft">
        <image width="256" height="256" source="../textures/SpikeLeft.png"/>
    </tile>
    <tile id="9" type="SpikeRight">
        <image width="256" height="256" source="../textures/SpikeRight.png"/>
    </tile>
    <tile id="10" type="SpikeUp">
        <image width="256" height="256" source="../textures/SpikeUp.png"/>
    </tile>
</tileset>