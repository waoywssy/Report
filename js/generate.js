$(function() {
	$("#date").datepicker({
		dateFormat:'yy-mm-dd',
		beforeShow: function(input, datepicker) {
				setTimeout(function() {
					$(datepicker.dpDiv).css('zIndex', 100);
				}, 200);
			}
		});
	$("#fromdate").datepicker({
		dateFormat:'yy-mm-dd',
		beforeShow: function(input, datepicker) {
				setTimeout(function() {
					$(datepicker.dpDiv).css('zIndex', 100);
				}, 200);
			}
		});
	$("#todate").datepicker({dateFormat:'yy-mm-dd',
		beforeShow: function(input, datepicker) {
				setTimeout(function() {
					$(datepicker.dpDiv).css('zIndex', 100);
				}, 200);
			}
		});
    
    $("button").button();
	/*
    $("button#generate").click(function(){
		$("#datesform").submit();
    });
	*/
	// validate signup form on keyup and submit
	$("#datesform").validate({
		rules: {
			fromdate: {
				required: true,
				minlength: 2
			},
			todate: {
				required: true,
				minlength: 5
			},
		},
		messages: {
			fromdate: {
				required: "Please enter a from date",
				minlength: "Your from date must like 'yyyy-mm-dd'"
			},
			todate: {
				required: "Please enter a to date",
				minlength: "Your to date must like 'yyyy-mm-dd'"
			}
		}
	});

	$("#dateform").validate({
		rules: {
			date: {
				required: true,
				minlength: 2
			},
		},
		messages: {
			date: {
				required: "Please enter a from date",
				minlength: "Your from date must like 'yyyy-mm-dd'"
			},
		}
	});
});