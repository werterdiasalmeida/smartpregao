!function (e, t) {
    "object" == typeof exports && "object" == typeof module ? module.exports = t(require("moment"), require("fullcalendar")) : "function" == typeof define && define.amd ? define(["moment", "fullcalendar"], t) : "object" == typeof exports ? t(require("moment"), require("fullcalendar")) : t(e.moment, e.FullCalendar)
}("undefined" != typeof self ? self : this, function (e, t) {
    return function (e) {
        function t(r) {
            if (o[r]) return o[r].exports;
            var a = o[r] = {i: r, l: !1, exports: {}};
            return e[r].call(a.exports, a, a.exports, t), a.l = !0, a.exports
        }

        var o = {};
        return t.m = e, t.c = o, t.d = function (e, o, r) {
            t.o(e, o) || Object.defineProperty(e, o, {configurable: !1, enumerable: !0, get: r})
        }, t.n = function (e) {
            var o = e && e.__esModule ? function () {
                return e.default
            } : function () {
                return e
            };
            return t.d(o, "a", o), o
        }, t.o = function (e, t) {
            return Object.prototype.hasOwnProperty.call(e, t)
        }, t.p = "", t(t.s = 171)
    }({
        0: function (t, o) {
            t.exports = e
        }, 1: function (e, o) {
            e.exports = t
        }, 171: function (e, t, o) {
            Object.defineProperty(t, "__esModule", {value: !0}), o(172);
            var r = o(1);
            r.datepickerLocale("pt-br", "pt-BR", {
                closeText: "Fechar",
                prevText: "&#x3C;Anterior",
                nextText: "Pr�ximo&#x3E;",
                currentText: "Hoje",
                monthNames: ["Janeiro", "Fevereiro", "Mar�o", "Abril", "Maio", "Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"],
                monthNamesShort: ["Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"],
                dayNames: ["Domingo", "Segunda-feira", "Ter�a-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "S�bado"],
                dayNamesShort: ["Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "S�b"],
                dayNamesMin: ["Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "S�b"],
                weekHeader: "Sm",
                dateFormat: "dd/mm/yy",
                firstDay: 0,
                isRTL: !1,
                showMonthAfterYear: !1,
                yearSuffix: ""
            }), r.locale("pt-br", {
                buttonText: {month: "M�s", week: "Semana", day: "Dia", list: "Compromissos"},
                allDayText: "dia inteiro",
                eventLimitText: function (e) {
                    return "mais +" + e
                },
                noEventsMessage: "N�o h� eventos para mostrar"
            })
        }, 172: function (e, t, o) {
            !function (e, t) {
                t(o(0))
            }(0, function (e) {
                return e.defineLocale("pt-br", {
                    months: "janeiro_fevereiro_mar�o_abril_maio_junho_julho_agosto_setembro_outubro_novembro_dezembro".split("_"),
                    monthsShort: "jan_fev_mar_abr_mai_jun_jul_ago_set_out_nov_dez".split("_"),
                    weekdays: "Domingo_Segunda-feira_Ter�a-feira_Quarta-feira_Quinta-feira_Sexta-feira_S�bado".split("_"),
                    weekdaysShort: "Dom_Seg_Ter_Qua_Qui_Sex_S�b".split("_"),
                    weekdaysMin: "Do_2�_3�_4�_5�_6�_S�".split("_"),
                    weekdaysParseExact: !0,
                    longDateFormat: {
                        LT: "HH:mm",
                        LTS: "HH:mm:ss",
                        L: "DD/MM/YYYY",
                        LL: "D [de] MMMM [de] YYYY",
                        LLL: "D [de] MMMM [de] YYYY [�s] HH:mm",
                        LLLL: "dddd, D [de] MMMM [de] YYYY [�s] HH:mm"
                    },
                    calendar: {
                        sameDay: "[Hoje �s] LT",
                        nextDay: "[Amanh� �s] LT",
                        nextWeek: "dddd [�s] LT",
                        lastDay: "[Ontem �s] LT",
                        lastWeek: function () {
                            return 0 === this.day() || 6 === this.day() ? "[�ltimo] dddd [�s] LT" : "[�ltima] dddd [�s] LT"
                        },
                        sameElse: "L"
                    },
                    relativeTime: {
                        future: "em %s",
                        past: "%s atr�s",
                        s: "poucos segundos",
                        ss: "%d segundos",
                        m: "um minuto",
                        mm: "%d minutos",
                        h: "uma hora",
                        hh: "%d horas",
                        d: "um dia",
                        dd: "%d dias",
                        M: "um m�s",
                        MM: "%d meses",
                        y: "um ano",
                        yy: "%d anos"
                    },
                    dayOfMonthOrdinalParse: /\d{1,2}�/,
                    ordinal: "%d�"
                })
            })
        }
    })
});