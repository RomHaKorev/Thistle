function foo( s, h ) {
	var div = document.getElementsByName(s);
	for ( var i =0; i < div.length; i++ ) {
		div[i].style.display="block";
	}
	var div = document.getElementsByName(h);
	for ( var i =0; i < div.length; i++ ) {
		div[i].style.display="none";
	}
} 