function I = read_ccs_data_file(filename, type)
%
% READ_CCS_DATA_FILE   import Code Composer Studio data file
%    I = READ_CCS_DATA_FILE(filename, type)
%
%    I: square image, may be UINT8, INT8, UINT16, or INT16
%    filename: CCStudio data file, must be hex format
%    type: data type, must be one of 'uint8', 'int8', 'uint16', 'int16'
%
% NOTE: this function assumes that the CCStudio data file contains
%       pixel data for a square image.
%

if strcmp(type, 'uint8') | strcmp(type, 'int8')
    nbytes = 4; % each word has 4 bytes
elseif strcmp(type, 'uint16') | strcmp(type, 'int16')
    nbytes = 2; % 2 bytes per word
else
    error('Please specify a desired data-type, [u]int8 or [u]int16');
end

fid = fopen(filename, 'r');
if -1 == fid
    error(['Could not open ' filename]);
end

tline = fgetl(fid);
% last token in header gives us the length
header_data = sscanf(tline, '%x');
len = header_data(length(header_data));
if (0 == len)
    % data length not defined, so compute it
    % from the # of text lines in the data file.
    fclose(fid); % will reopen afterwards
    len = how_many_lines(filename);
    % get back to where we were prior to this
    % diversion.
    fid = fopen(filename, 'r');
    tline = fgetl(fid);
end
N = sqrt(len*nbytes); % assumes square image matrix!
I = zeros(N, N);
for ii = 1:N
    if 2==nbytes % 16 bpp
        for jj = 1:N/2
            tline = fgetl(fid);
            I(ii, 2*jj-1:2*jj) = ... % fliplr for endianness
                fliplr([sscanf(tline(3:6),'%x') sscanf(tline(7:10),'%x')]);
        end
    else % 8 bpp
        for jj = 1:N/4
            tline = fgetl(fid);
            % little endian format
            p1 = tline(9:10);
            p2 = tline(7:8);
            p3 = tline(5:6);
            p4 = tline(3:4);
            I(ii, 4*(jj-1)+1:4*jj) = ...
                [sscanf(p1,'%x') sscanf(p2,'%x') sscanf(p3,'%x') sscanf(p4,'%x')];
        end
    end
end

fclose(fid);

if 'u' ~= type(1) % unsigned-to-signed integer conversion
    halfway = 2^(32/nbytes-1)-1; % e.g., 127 for 8 bpp
    % find and replace those values that are actually negative
    knegative = find(I>halfway);
    offset = zeros(size(I));
    offset(knegative) = bitshift(1,32/nbytes); % (1<<8) or 256
    I = I-offset;
end

eval( sprintf('I=%s(I);', type) );

function N = how_many_lines(filename)

fid = fopen(filename, 'r');
tline = fgetl(fid);
N = 0;
while 1
    tline = fgetl(fid);
    if ~ischar(tline), break, end
    N = N + 1;
end
fclose(fid);