data = fopen('print.txt');
%D = fread(data,'*char');
l = fgetl(data);
dict = {1010 'c_inc';
    1011 'b_inc';
    1012 'x_inc';
    1013 'y_inc';
    1014 'z_inc'
    1015 's_inc';
    1016 'c_abs'
    1017 'b_abs';
    1018 'x_abs';
    1019 'y_abs';
    1020 'z_abs';
    1021 's_abs';
    1022 'g5p2';
    1023 's_loop';
    1024 'x_loop';
    };
%D = {}; ndx = 1;
%%
D = {}; ndx = 1;
while ~feof(data)
    l = fgetl(data);
    if l == '%'
        continue;
    end
    switch lower(l(1:3))
        case 'sub'
            %D{ndx,1} = l(5:end);
            
            %sub
            sub = str2num(l(5:end))-1009;
            D{ndx,1} = dict{sub,2};
            
            %feedrate
            l = fgetl(data);
            D{ndx,2} = str2num(l(9:end));
            
            %time
            l = fgetl(data);
            D{ndx,4} = str2num(l(5:end));
            
            %smc
            l = fgetl(data);
            D{ndx,3} = str2num(l(7:end));
            
            ndx = ndx + 1;
            
    end
    
end
xlswrite('data.xls',sortrows(D));
%[trash idx] = sort([D{:,1}], 'descend');
%D(idx,:)