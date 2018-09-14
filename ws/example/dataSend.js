function dataSend() {
    //alert('success!');
    var ori = document.getElementsByTagName('input');
    for (var i=0; i<ori.length; i++) {
    	// 获得各个文本输入框的字段名和值
    	var name = ori[i].name;
    	var value = ori[i].value;
    	ws.send()
    }

}

<script type="text/javascript" src="dataSend.js"></script>