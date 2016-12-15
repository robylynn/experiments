info = fopen('1005.EIA');
data = fopen('print.txt');
%D = fread(data,'*char');
l = fgetl(data);
dict = {1010 'normal';
    1011 '0015_dec';
    1012 '005_dec';
    1013 '01_dec';
    1014 '025_dec'
    1015 '0015_smooth';
    1016 '005_smooth'
    1017 '01_smooth';
    1018 '025_smooth';
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