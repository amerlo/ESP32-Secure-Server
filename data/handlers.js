$(function() {
    $.get("api/status", function(data) {
        jQuery(data).each(function(i, relay) {
            var checked = relay.value ? "checked" : "";
            var item = `<li id="${relay.id}" class="list-group-item">
                <div class="row">
                    <div class="col-10">
                        <label class="form-check-label">${relay.name} - <i>GPIO ${relay.gpio}</i></label>
                    </div>
                    <div class="col-2 d-flex justify-content-center">
                        <div class="spinner visually-hidden" style="width: 30px;">
                            <div class="spinner-border spinner-border-sm text-secondary" role="status"></div>
                        </div>
                        <div class="form-check form-switch" style="margin-bottom: 0;">
                            <input class="form-check-input shadow-none" type="checkbox" id="${relay.id}" onchange="handleChange(event)" ${checked} />
                        </div>
                    </div>
                </div>
            </li>`;

            $("#relays").append(item);
        });

        $("#loadingSpinner").addClass("visually-hidden");
        $("#relaysCard").removeClass("visually-hidden");
    });
});

function handleChange(e) {
    var relayId = e.target.id;
    var relayValue = e.target.checked;

    $("li#"+relayId+" .form-switch").addClass("visually-hidden");
    $("li#"+relayId+" .spinner").removeClass("visually-hidden");
    
    $.post("api/update", `{ "id": ${relayId}, "value": ${relayValue} }`)
        .fail(function() {
            $("li#"+relayId+" input").prop("checked", !relayValue);
            $('.toast').toast('show');
        })
        .always(function() {
            $("li#"+relayId+" .spinner").addClass("visually-hidden");
            $("li#"+relayId+" .form-switch").removeClass("visually-hidden");
            $("li#"+relayId+" input").blur();
        });
}
