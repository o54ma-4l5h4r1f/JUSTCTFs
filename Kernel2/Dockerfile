FROM o54ma4l5h4r1f/pwn-gcc3 AS pwn

FROM pwn.red/jail
COPY --from=pwn / /srv

COPY kernel2 /srv/app/run
COPY flag.txt /srv/app/flag.txt

ENV JAIL_TIME=0