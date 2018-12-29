import QtQuick 2.2
import QtQuick.Layouts 1.11

Item
{
    id: rootItem
    property alias errorMessage: text.text
    Column
    {
        id: column
        anchors.centerIn: parent
        Rectangle
        {
            id: rect
            anchors.horizontalCenter: column.horizontalCenter
            function size()
            {
                return Math.min(rootItem.width, rootItem.height - text.height)
            }
            width: size()
            height: size()

            Image
            {
                visible: false
                id: img
                anchors.fill: parent
                scale: Image.PreserveAspectFit
                source: "../icons/saddie.png"
                clip: true
            }

            ShaderEffect {
                anchors.fill: parent
                property variant src: img
                vertexShader: "
                    uniform highp mat4 qt_Matrix;
                    attribute highp vec4 qt_Vertex;
                    attribute highp vec2 qt_MultiTexCoord0;
                    varying highp vec2 coord;
                    void main() {
                        coord = qt_MultiTexCoord0;
                        gl_Position = qt_Matrix * qt_Vertex;
                    }"
                fragmentShader: "
                    varying highp vec2 coord;
                    uniform sampler2D src;
                    uniform lowp float qt_Opacity;
                    void main() {
                        lowp vec4 tex = texture2D(src, coord);
                        gl_FragColor = vec4(vec3(dot(tex.rgb,
                                            vec3(0.344, 0.5, 0.156))),
                                                 tex.a) * qt_Opacity;
                    }"
            }
        }
        Text
        {
            id: text
            anchors.horizontalCenter: parent.horizontalCenter
            width: rootItem.width
            font.bold: true
            wrapMode: Text.WordWrap
            text: "<Placeholder>"
            color: "red"
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
