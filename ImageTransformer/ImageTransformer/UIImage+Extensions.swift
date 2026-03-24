//
//  UIImage+Extensions.swift
//  ImageTransformer
//
//  Created by Miraslau Parafeniuk on 23.03.26.
//

import Foundation
import Cocoa
import CoreGraphics
import ImageIO

enum ImageMatrixError: Error {
    case cannotLoadImage
    case cannotCreateContext
    case cannotGetData
}

func grayscaleMatrix(from url: URL) throws -> [[Int]] {
    guard
        let source = CGImageSourceCreateWithURL(url as CFURL, nil),
        let cgImage = CGImageSourceCreateImageAtIndex(source, 0, nil)
    else {
        throw ImageMatrixError.cannotLoadImage
    }

    let width = cgImage.width
    let height = cgImage.height
    let bytesPerPixel = 1
    let bytesPerRow = width * bytesPerPixel
    let bitsPerComponent = 8

    let colorSpace = CGColorSpaceCreateDeviceGray()

    guard let context = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: bitsPerComponent,
        bytesPerRow: bytesPerRow,
        space: colorSpace,
        bitmapInfo: CGImageAlphaInfo.none.rawValue
    ) else {
        throw ImageMatrixError.cannotCreateContext
    }

    let rect = CGRect(x: 0, y: 0, width: width, height: height)
    context.draw(cgImage, in: rect)

    guard let data = context.data else {
        throw ImageMatrixError.cannotGetData
    }

    let buffer = data.bindMemory(to: UInt8.self, capacity: width * height)

    var matrix: [[Int]] = []
    matrix.reserveCapacity(height)

    for y in 0..<height {
        var row: [Int] = []
        row.reserveCapacity(width)

        for x in 0..<width {
            let value = Int(buffer[y * bytesPerRow + x])
            row.append(value)
        }

        matrix.append(row)
    }

    return matrix
}

enum MatrixToImageError: Error {
    case emptyMatrix
    case inconsistentRowLengths
    case valueOutOfRange(x: Int, y: Int, value: Int)
    case cannotCreateContext
    case cannotCreateImage
}

func image(from matrix: [[Int]], scale: Int) throws -> NSImage {
    guard !matrix.isEmpty, !matrix[0].isEmpty else {
        throw MatrixToImageError.emptyMatrix
    }

    let height = matrix.count
    let width = matrix[0].count

    for y in 0..<height {
        guard matrix[y].count == width else {
            throw MatrixToImageError.inconsistentRowLengths
        }
    }

    var pixels = [UInt8]()
    pixels.reserveCapacity(width * height)

    for y in 0..<height {
        for x in 0..<width {
            let value = matrix[y][x] * scale
            guard (0...255).contains(value) else {
                throw MatrixToImageError.valueOutOfRange(x: x, y: y, value: value)
            }
            pixels.append(UInt8(value))
        }
    }

    let bitsPerComponent = 8
    let bytesPerPixel = 1
    let bytesPerRow = width * bytesPerPixel
    let colorSpace = CGColorSpaceCreateDeviceGray()

    guard let context = CGContext(
        data: &pixels,
        width: width,
        height: height,
        bitsPerComponent: bitsPerComponent,
        bytesPerRow: bytesPerRow,
        space: colorSpace,
        bitmapInfo: CGImageAlphaInfo.none.rawValue
    ) else {
        throw MatrixToImageError.cannotCreateContext
    }

    guard let cgImage = context.makeImage() else {
        throw MatrixToImageError.cannotCreateImage
    }

    return NSImage(cgImage: cgImage, size: NSSize(width: width, height: height))
}
