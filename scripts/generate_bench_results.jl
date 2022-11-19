using Plots
using LaTeXStrings
using Unitful

default(fontfamily="Computer Modern")

fmt_x(n) = L"10^%$n"

n = 1:9
own = uconvert.(u"s", u"ms" .* [1, 1, 1, 1, 19, 357, 6758, 109909, 1903862])
ycruncher = uconvert.(u"s", u"ms" .* [7, 11, 13, 8, 22, 281, 4354, 71186, 1061700])

plt = plot(xticks=(n, fmt_x.(n)), xlabel="Digits of π", yformatter=string, legend=:topleft, dpi=300)
plot!(plt, own, label="Our program")
plot!(plt, ycruncher, label="y-cruncher", ylabel="Time to complete")

savefig(plt, "bench.png")
