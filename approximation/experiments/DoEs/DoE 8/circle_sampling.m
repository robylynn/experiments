pts = 0:.001:2*pi;
%ciecle radius
r = 4;
X = r*cos(pts);
Y = r*sin(pts);
out = fopen('circle.txt','w+');
for j = 1:length(X)
    fprintf(out,'X%3.4f Y%3.4f\n',X(j),Y(j));
end